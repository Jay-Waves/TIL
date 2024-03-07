## 实现原理

### 优化

[AddressSanitizerCompileTimeOptimizations](https://github.com/google/sanitizers/wiki/AddressSanitizerCompileTimeOptimizations)

### 插桩阶段 (Instrumentation Pass)

ASAN 实现代码在 `/llvm-project/llvm/lib/Transforms/Instrumentation/AddressSanitizer.cpp`. FunctionPass 遍历所有函数进行插桩 `instrumentFunciton()`

```cpp
bool AddressSanitizer::instrumentFunction(Function &F,const TargetLibraryInfo *TLI) {
...
 // 这些Vector用于保存筛选出来的指令对象和信息
  SmallVector<InterestingMemoryOperand, 16> OperandsToInstrument;
  SmallVector<MemIntrinsic *, 16> IntrinToInstrument;
  SmallVector<BasicBlock *, 16> AllBlocks;
  int NumAllocas = 0;

  for (auto &BB : F) {
    AllBlocks.push_back(&BB);
    for (auto &Inst : BB) {
      SmallVector<InterestingMemoryOperand, 1> InterestingOperands;
      getInterestingMemoryOperands(&Inst, InterestingOperands);

// 如果当前指令属于需要插桩的位置,那就记录一下,后面会用到
      if (!InterestingOperands.empty()) {  
        for (auto &Operand : InterestingOperands) {
          OperandsToInstrument.push_back(Operand);
        }
      } else if (MemIntrinsic *MI = dyn_cast<MemIntrinsic>(&Inst)) {  
	    // memset/memcpy/memmove
        IntrinToInstrument.push_back(MI);
      }
    }
  }
...
    
  for (auto &Operand : OperandsToInstrument) {  // 对数据访问指令进行操作
    instrumentMop(ObjSizeVis, Operand, UseCalls,
                    F.getParent()->getDataLayout());
    FunctionModified = true;
  }
  for (auto Inst : IntrinToInstrument) {  // 对内存操作指令进行操作
    instrumentMemIntrinsic(Inst);
    FunctionModified = true;
  }

  FunctionStackPoisoner FSP(F, *this);
  bool ChangedStack = FSP.runOnFunction();  // 对插桩之后的函数进行栈调整
    
  // ...
    
  return FunctionModified;
}

void AddressSanitizer::getInterestingMemoryOperands(
    Instruction *I, SmallVectorImpl<InterestingMemoryOperand> &Interesting) {
  if (LoadInst *LI = dyn_cast<LoadInst>(I)) {  // LLVM IR Load指令,用于读取数据
    if (ignoreAccess(LI->getPointerOperand()))  // 判断指令中的操作数是否为指针
      return;
    Interesting.emplace_back(I, LI->getPointerOperandIndex(), false,
                             LI->getType(), LI->getAlign());
  } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {  // LLVM IR Store指令,用于保存数据
    if (ignoreAccess(SI->getPointerOperand()))
      return;
    Interesting.emplace_back(I, SI->getPointerOperandIndex(), true,
                             SI->getValueOperand()->getType(), SI->getAlign());
  }
}
```

筛选出需要插桩的指令后, 在 `LLVM IR Load/Store` 指令前插入异常检测逻辑, 即如果没有异常才能够执行原本的读写操作, 否则 ASAN 报告异常.

```cpp
void AddressSanitizer::instrumentMop(
									ObjectSizeOffsetVisitor &ObjSizeVis,
									InterestingMemoryOperand &O, 
									bool UseCalls,const DataLayout &DL
) {
// 获取指令操作的指针地址
  Value *Addr = O.getPtr();  
...
// 内存检测粒度,后续再详解
  unsigned Granularity = 1 << Mapping.Scale;  
    
  doInstrumentAddress(this, O.getInsn(), O.getInsn(), Addr, O.Alignment,
                  Granularity, O.TypeSize, O.IsWrite, nullptr, UseCalls,
                  Exp);
}

static void doInstrumentAddress(AddressSanitizer *Pass, Instruction *I,
                                Instruction *InsertBefore, Value *Addr,
                                MaybeAlign Alignment, unsigned Granularity,
                                uint32_t TypeSize, bool IsWrite,
                                Value *SizeArgument, bool UseCalls,
                                uint32_t Exp
) {

  if ((TypeSize == 8 || TypeSize == 16 || TypeSize == 32 || TypeSize == 64 ||
       TypeSize == 128) &&
      (!Alignment || *Alignment >= Granularity || *Alignment >= TypeSize / 8))
    // 当前指令访问方式是按 8 字节对齐的
    return Pass->instrumentAddress(I, InsertBefore, Addr, TypeSize, IsWrite,
                                   nullptr, UseCalls, Exp);  
  Pass->instrumentUnusualSizeOrAlignment(I, InsertBefore, Addr, TypeSize,
                                         IsWrite, nullptr, UseCalls, Exp);
}

void AddressSanitizer::instrumentAddress(Instruction *OrigIns,Instruction *InsertBefore, Value *Addr,uint32_t TypeSize, bool IsWrite,Value *SizeArgument, bool UseCalls,uint32_t Exp) {
  bool IsMyriad = TargetTriple.getVendor() == llvm::Triple::Myriad;

  IRBuilder<> IRB(InsertBefore);  // LLVM IR指令生成器
  Value *AddrLong = IRB.CreatePointerCast(Addr, IntptrTy);
  size_t AccessSizeIndex = TypeSizeToSizeIndex(TypeSize);

  Type *ShadowTy =
      IntegerType::get(*C, std::max(8U, TypeSize >> Mapping.Scale));
  Type *ShadowPtrTy = PointerType::get(ShadowTy, 0);
  Value *ShadowPtr = memToShadow(AddrLong, IRB);
  Value *CmpVal = Constant::getNullValue(ShadowTy);
  Value *ShadowValue =
      IRB.CreateLoad(ShadowTy, IRB.CreateIntToPtr(ShadowPtr, ShadowPtrTy));

  Value *Cmp = IRB.CreateICmpNE(ShadowValue, CmpVal);
  Instruction *CrashTerm = nullptr;

   /*
   上面这段指令生成的意思是创建if判断:
  shadow_page_flag = *(_BYTE *)((((unsigned __int64)real_data + 0x1001) >> 3) + 0x7FFF8000);
  real_data_offset = (unsigned __int64)real_data + 0x1001;
   if ( shadow_page_flag )  // ASAN内存异常检测插桩判断
   */
    
  CrashTerm = SplitBlockAndInsertIfThen(Cmp, InsertBefore, !Recover);
  Instruction *Crash = generateCrashCode(CrashTerm, AddrLong, IsWrite, AccessSizeIndex, SizeArgument, Exp);
    
   /*
   上面这段指令生成的意思是if判断成功时,在它的子BasicBlock中创建函数调用:
    _asan_report_store1(v13);  // 提示报错
    
   所以合并起来插桩代码就是:
  shadow_page_flag = *(_BYTE *)((((unsigned __int64)real_data + 0x1001) >> 3) + 0x7FFF8000);
  real_data_offset = (unsigned __int64)real_data + 0x1001;
   if ( shadow_page_flag )  // ASAN内存异常检测插桩判断
    _asan_report_store1(real_data_offset);  // 提示报错
   */
}
```

调整函数的栈空间, 把 ShadowTable 的分配和销毁引入进来.

```cpp
bool AddressSanitizer::instrumentFunction() {
   // ...
   FunctionStackPoisoner FSP(F, *this);
   bool ChangedStack = FSP.runOnFunction();
   // ...
}

bool runOnFunction() {
   // ...
   // 遍历函数中所有指令,筛选出内存分配操作
   for (BasicBlock *BB : depth_first(&F.getEntryBlock())) visit(*BB);
   // ...
   processDynamicAllocas();
   processStaticAllocas();
   // ...

   return true;
}

void visitAllocaInst(AllocaInst &AI) {  // 遍历指令时遇到AllocaInst,它的意义是在栈内分配指定大小内存
  if (!AI.isStaticAlloca())  // 只要在当前函数声明的变量,无论在if/switch/while/for里面哪个BasicBlock,编译时都会把这块内存的申请放到函数的入口BasicBlock中.isStaticAlloca的用意就在于判断这个AllocInst是否在当前函数的入口BasicBlock中执行,而且还判断AllocInst创建的内存大小的值是否会变而不是指定的大小.
    DynamicAllocaVec.push_back(&AI);
  else
    AllocaVec.push_back(&AI);
}

void visitIntrinsicInst(IntrinsicInst &II) {
  bool DoPoison = (ID == Intrinsic::lifetime_end);
  AllocaPoisonCall APC = {&II, AI, SizeValue, DoPoison};
  if (AI->isStaticAlloca())  // 同上
    StaticAllocaPoisonCallVec.push_back(APC);  // 记录栈中分配对象大小和偏移信息
  else if (ClInstrumentDynamicAllocas)
    DynamicAllocaPoisonCallVec.push_back(APC);
}
```

```cpp
void FunctionStackPoisoner::processStaticAllocas() {
  // ...
  Instruction *InsBefore = AllocaVec[0];
  IRBuilder<> IRB(InsBefore);  // 在函数的第一个AllocaInst指令前插入新代码

  SmallVector<ASanStackVariableDescription, 16> SVD;
  SVD.reserve(AllocaVec.size());
  for (AllocaInst *AI : AllocaVec) {  // 遍历所有在函数入口点声明的AllocaInst指令,收集这些AllocaInst指令的信息
    ASanStackVariableDescription D = {AI->getName().data(),
                                      ASan.getAllocaSizeInBytes(*AI),
                                      0,
                                      AI->getAlignment(),
                                      AI,
                                      0,
                                      0};
    SVD.push_back(D);
  }

  size_t Granularity = 1ULL << Mapping.Scale;  // 内存粒度,后面再具体说明
  size_t MinHeaderSize = std::max((size_t)ASan.LongSize / 2, Granularity);
  const ASanStackFrameLayout &L =
      ComputeASanStackFrameLayout(SVD, Granularity, MinHeaderSize);  // 调整ASAN插桩后的整个栈布局
  uint64_t LocalStackSize = L.FrameSize;  // 获取调整之后的栈布局大小
    
  Value *StaticAlloca =
      DoDynamicAlloca ? nullptr : createAllocaForLayout(IRB, L, false);  // 调整新栈空间,这块栈内存是真实使用的
  Value *FakeStack;
  Value *LocalStackBase;
  Value *LocalStackBaseAlloca;
  uint8_t DIExprFlags = DIExpression::ApplyOffset;

  LocalStackBaseAlloca =
      IRB.CreateAlloca(IntptrTy, nullptr, "asan_local_stack_base");
  Constant *OptionDetectUseAfterReturn = F.getParent()->getOrInsertGlobal(
      kAsanOptionDetectUseAfterReturn, IRB.getInt32Ty());
  Value *UseAfterReturnIsEnabled = IRB.CreateICmpNE(
      IRB.CreateLoad(IRB.getInt32Ty(), OptionDetectUseAfterReturn),
      Constant::getNullValue(IRB.getInt32Ty()));
  Instruction *Term =
      SplitBlockAndInsertIfThen(UseAfterReturnIsEnabled, InsBefore, false);
  IRBuilder<> IRBIf(Term);
  StackMallocIdx = StackMallocSizeClass(LocalStackSize);
  assert(StackMallocIdx <= kMaxAsanStackMallocSizeClass);
  Value *FakeStackValue =
      IRBIf.CreateCall(AsanStackMallocFunc[StackMallocIdx],
                       ConstantInt::get(IntptrTy, LocalStackSize));
  IRB.SetInsertPoint(InsBefore);
  FakeStack = createPHI(IRB, UseAfterReturnIsEnabled, FakeStackValue, Term,
                        ConstantInt::get(IntptrTy, 0));

  Value *NoFakeStack =
      IRB.CreateICmpEQ(FakeStack, Constant::getNullValue(IntptrTy));
  Term = SplitBlockAndInsertIfThen(NoFakeStack, InsBefore, false);
  IRBIf.SetInsertPoint(Term);
  Value *AllocaValue =
      DoDynamicAlloca ? createAllocaForLayout(IRBIf, L, true) : StaticAlloca;

  IRB.SetInsertPoint(InsBefore);
  LocalStackBase = createPHI(IRB, NoFakeStack, AllocaValue, Term, FakeStack);
  IRB.CreateStore(LocalStackBase, LocalStackBaseAlloca);
  // 生成的插桩代码等价于:
  // void *FakeStack = __asan_option_detect_stack_use_after_return
  //     ? __asan_stack_malloc_N(LocalStackSize)
  //     : nullptr;
  // void *LocalStackBase = (FakeStack) ? FakeStack : alloca(LocalStackSize);
  // 意思是从ShadowTable中分配一块栈内存,这块栈内存是用于异常检测的.__asan_stack_malloc_N()的实现代码在Compiler-RT.

  Value *LocalStackBaseAllocaPtr =
      isa<PtrToIntInst>(LocalStackBaseAlloca)
          ? cast<PtrToIntInst>(LocalStackBaseAlloca)->getPointerOperand()
          : LocalStackBaseAlloca;  // 获取ShadowTable中的栈起始地址

  for (const auto &Desc : SVD) {  // 根据AllocaInst的申请栈分配内存大小和位置,在ShadowTable中重新调整到对应的位置
    AllocaInst *AI = Desc.AI;
    Value *NewAllocaPtr = IRB.CreateIntToPtr(
        IRB.CreateAdd(LocalStackBase, ConstantInt::get(IntptrTy, Desc.Offset)),
        AI->getType());
    AI->replaceAllUsesWith(NewAllocaPtr);
  }

  // 这些插桩代码都不太重要,意义就是在ShadowTable中创建的栈内存记录当前函数的信息
  Value *BasePlus0 = IRB.CreateIntToPtr(LocalStackBase, IntptrPtrTy);
  IRB.CreateStore(ConstantInt::get(IntptrTy, kCurrentStackFrameMagic),
                  BasePlus0);
  // Write the frame description constant to redzone[1].
  Value *BasePlus1 = IRB.CreateIntToPtr(
      IRB.CreateAdd(LocalStackBase,
                    ConstantInt::get(IntptrTy, ASan.LongSize / 8)),
      IntptrPtrTy);
  GlobalVariable *StackDescriptionGlobal =
      createPrivateGlobalForString(*F.getParent(), DescriptionString,
                                   /*AllowMerging*/ true, kAsanGenPrefix);
  Value *Description = IRB.CreatePointerCast(StackDescriptionGlobal, IntptrTy);
  IRB.CreateStore(Description, BasePlus1);
  // Write the PC to redzone[2].
  Value *BasePlus2 = IRB.CreateIntToPtr(
      IRB.CreateAdd(LocalStackBase,
                    ConstantInt::get(IntptrTy, 2 * ASan.LongSize / 8)),
      IntptrPtrTy);
  IRB.CreateStore(IRB.CreatePointerCast(&F, IntptrTy), BasePlus2);

  const auto &ShadowAfterScope = GetShadowBytesAfterScope(SVD, L);  // 根据SVD中记录栈中各个变量对应的内存位置初始化ShadowTable的栈内存

  Value *ShadowBase = ASan.memToShadow(LocalStackBase, IRB);  // ASan.memToShadow()用于计算进程内存在ShadowTable的偏移位置
  copyToShadow(ShadowAfterScope, ShadowAfterScope, IRB, ShadowBase);  // 21给函数栈内存投毒

  if (!StaticAllocaPoisonCallVec.empty()) {  // 2.对栈中分配的变量在ShadowTable中消毒
    const auto &ShadowInScope = GetShadowBytes(SVD, L);

    for (const auto &APC : StaticAllocaPoisonCallVec) {
      const ASanStackVariableDescription &Desc = *AllocaToSVDMap[APC.AI];
      assert(Desc.Offset % L.Granularity == 0);
      size_t Begin = Desc.Offset / L.Granularity;
      size_t End = Begin + (APC.Size + L.Granularity - 1) / L.Granularity;

      IRBuilder<> IRB(APC.InsBefore);
      copyToShadow(ShadowAfterScope,
                   APC.DoPoison ? ShadowAfterScope : ShadowInScope, Begin, End,
                   IRB, ShadowBase);
    }
  }
  /*
  投毒再消毒后,ShadowTable的内存数据布局如下:
  1.ShadowTable分配栈后对内存投毒 =>  F3F3F8F8F1F1F1F1
  2.对栈中需要用到的变量位置消毒  =>  F3F30000F1F1F1F1
  此时访问栈变量,获取到的数据就是0x00,为正常数据访问;如果是不允许访问的话,那就必定不为0
  */
    
  SmallVector<uint8_t, 64> ShadowClean(ShadowAfterScope.size(), 0);
  SmallVector<uint8_t, 64> ShadowAfterReturn;

  for (auto Ret : RetVec) {
    IRBuilder<> IRBRet(Ret);
    // Mark the current frame as retired.
    IRBRet.CreateStore(ConstantInt::get(IntptrTy, kRetiredStackFrameMagic),
                       BasePlus0);
      
    // 简单总结就是在函数返回时清空ShadowTable中的栈数据为0xF5
    // if FakeStack != 0  // LocalStackBase == FakeStack
    //     // In use-after-return mode, poison the whole stack frame.
    //     if StackMallocIdx <= 4
    //         // For small sizes inline the whole thing:
    //         memset(ShadowBase, kAsanStackAfterReturnMagic, ShadowSize);
    //         **SavedFlagPtr(FakeStack) = 0
    //     else
    //         __asan_stack_free_N(FakeStack, LocalStackSize)
    // else
    //     <This is not a fake stack; unpoison the redzones>
    Value *Cmp =
        IRBRet.CreateICmpNE(FakeStack, Constant::getNullValue(IntptrTy));
    Instruction *ThenTerm, *ElseTerm;
    SplitBlockAndInsertIfThenElse(Cmp, Ret, &ThenTerm, &ElseTerm);

    IRBuilder<> IRBPoison(ThenTerm);
    if (StackMallocIdx <= 4) {
      int ClassSize = kMinStackMallocSize << StackMallocIdx;
      ShadowAfterReturn.resize(ClassSize / L.Granularity,
                               kAsanStackUseAfterReturnMagic);
      copyToShadow(ShadowAfterReturn, ShadowAfterReturn, IRBPoison,
                   ShadowBase);
      Value *SavedFlagPtrPtr = IRBPoison.CreateAdd(
          FakeStack,
          ConstantInt::get(IntptrTy, ClassSize - ASan.LongSize / 8));
      Value *SavedFlagPtr = IRBPoison.CreateLoad(
          IntptrTy, IRBPoison.CreateIntToPtr(SavedFlagPtrPtr, IntptrPtrTy));
      IRBPoison.CreateStore(
          Constant::getNullValue(IRBPoison.getInt8Ty()),
          IRBPoison.CreateIntToPtr(SavedFlagPtr, IRBPoison.getInt8PtrTy()));
    } else {
      // For larger frames call __asan_stack_free_*.
      IRBPoison.CreateCall(
          AsanStackFreeFunc[StackMallocIdx],
          {FakeStack, ConstantInt::get(IntptrTy, LocalStackSize)});
    }

    IRBuilder<> IRBElse(ElseTerm);
    copyToShadow(ShadowAfterScope, ShadowClean, IRBElse, ShadowBase);
  }
}
```

### 运行时逻辑 (Compiler-RT)

ASan 有大量内存分配/操作功能, 为了避免 Pass 过于臃肿, 将一些核心功能写在了 Compiler-RT 中, 让 Clang 在链接阶段引入它们.

```cpp
void __asan_init() {
  AsanActivate();
  AsanInitInternal();
}

void AsanActivate() {
  asan_deactivated_flags.OverrideFromActivationFlags();  // 从环境变量ASAN_ACTIVATION_OPTIONS中获取ASAN配置

  SetCanPoisonMemory(asan_deactivated_flags.poison_heap);
  SetMallocContextSize(asan_deactivated_flags.malloc_context_size);
  ReInitializeAllocator(asan_deactivated_flags.allocator_options);
}

static void AsanInitInternal() {
  InitializeHighMemEnd();
  InitializeShadowMemory();

  AllocatorOptions allocator_options;
  allocator_options.SetFrom(flags(), common_flags());
  InitializeAllocator(allocator_options);

  InitializeCoverage(common_flags()->coverage, common_flags()->coverage_dir);
}
```
