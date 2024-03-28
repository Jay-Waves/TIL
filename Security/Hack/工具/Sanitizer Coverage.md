借助 sanitizer coverage 对程序分支进行简易插桩

```cpp
#include <stdlib.h>

int function1(int a) {
    if (1 == a)
        return 0;

    return 1;
}

int function2() {
    return -1;
}

int main() {
    if (rand() % 2)
        function1(rand() % 3);
    else
        function2();

    return 0;
}
```

插桩后程序:

```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v3; // eax
  __int64 v4; // rdx
  int v5; // eax

  _sanitizer_cov_trace_pc_guard(&unk_439BC0, argv, envp);
  v3 = rand();
  v4 = (unsigned int)(v3 >> 31);
  LODWORD(v4) = v3 % 2;
  if ( v3 % 2 )
  {
    _sanitizer_cov_trace_pc_guard((char *)&unk_439BC0 + 4, argv, v4);
    v5 = rand();
    function1(v5 % 3);
  }
  else
  {
    _sanitizer_cov_trace_pc_guard((char *)&unk_439BC0 + 8, argv, v4);
    function2();
  }
  return 0;
}
```

## 插桩函数

使用参数 `-fsanitizer-coverage=trace-pc-guard`, SanCov 可以将 `__sanitizer_cov_trace_pc_guard()` 插入到每个分支, 将 `__sanitizer_cov_trace_pc_guard_init()` 插入到每个 dso 起始. 这两个函数可以是用户自定义的.

```cpp
uint32_t __sancov_current_all_guard_count = 0;

void __sanitizer_cov_trace_pc_guard_init(uint32_t *start,uint32_t *stop) {
    __sancov_current_all_guard_count = (stop - start);
}

void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
  if (!*guard) return;
  void *PC = __builtin_return_address(0);
}
```

## API

插桩粒度, 需要和 trace-pc 插桩功能搭配使用:
- `func`, 函数调用插桩. 如 `-fsan...=func,trace-pc`
- `edge`, IR基本块分支插桩
- `bb`, IR基本块插桩
- `indirect-calls` -> `__sanitizer_cov_trace_pc_indirect(void *callee)` 对非直接函数调用进行插桩, 即函数指针. 如 `-fsan...=trace-pc,indirect-calls`. 似乎部分情况下, 实际插入的是 `__sanitizer_cov_trace_pc_indir()?`, 文档没说过

trace-pc 插桩功能: 
- `trace-pc` -> `__sanitizer_cov_trace_pc()` 仅对每个插桩点执行此callback, 无信息.
- `trace-pc-guard` -> `__sanitizer_cov_trace_pc_guard(*guard)` 对每个插桩点插入此 callback, 同时可用 guard 记录信息; `__sanitizer_cov_trace_pc_guard_init(*start, *stop)` 初始化每个DSO的guards. 
- `inline-8bit-counters` -> `__sanitizer_cov_8bit_counters_init(*start, *end)` 在每个插桩点插入一字节计数器, 用此 callback 初始化, 每次覆盖插桩点时计数器加一.
- `inline-bool-flag` -> `__sanitizer_cov_bool_flag_init(*start, *end)` 每个插桩点插入一个 bool, 覆盖时置为 true. 用此回调获取 flags 指针位置.
- `pc-table` 记录 PC 表, 需要搭配上面功能使用.

`no-prune` 不允许SanCov修剪掉一些无价值代码路径. (禁用后资源占用膨胀)

trace-dataflow 插桩功能, 需要和 trace-pc 插桩搭配使用:
- `...=trace-cmp`
- `trace-div`
- `trace-gep`: IR 数组/结构体访址指令, `getelementptr`
- `trace-stores`: 插桩 IR load/store 指令

trace-controlflow 插桩功能: ...

> clang/gcc 也支持最简单的插桩 `-finstrument-functions`

## 编译

假设定义插桩函数文件为 `inst_rt.o.cc`
1. 编译为链接文件: `clang++ -c inst_rt.o.cc -o inst_rt.o -fsanitize=address`
2. 编译被插桩文件, 记得将 hook 目标文件传入: `clang++ test.cc -g -o test -fsanitize=address -fsanitize-coverage=trace-pc-guard inst_rt.o`

## 禁用插桩

对于插桩函数 `__sanitizer_cov_trace_pc_guard()` 等函数可能调用的函数, 如果也开启 `coverage` 插桩选项, 可能导致无限递归. 此时可以使用编译器指令来禁用插桩: `__attribute__((no_sanitize("coverage")))`, 并搭配 `__has_feature(coverage_sanitizer)` (如果不使用 clang, 就没有 sancov 功能).

```cpp
#if __has_feature(coverage_sanitizer)
#define NO_SANCOV __attribute__((no_sanitize("coverage")))
#else
#define NO_SANCOV
#endif

NO_SANCOV
void my_own_callback(){
	...
}

__sanitizer_cov_trace_pc_guard(){
	my_own_callback();
}
```

## SanCov 实现

Sanitizer Coverage 使用 [LLVM Pass](../../../Compiler/LLVM/第一个Pass.md) 实现. 代码在 `llvm-project/llvm/lib/Transforms/Instrumentation/SanitizerCoverage.cpp` 下, 基于 Module Pass 实现子类. Module Pass 入口点是 `runOnModule()`, 这里主要将参数传递给 `instrumentModule()`

```cpp
class ModuleSanitizerCoverageLegacyPass : public ModulePass {
public:
  bool runOnModule(Module &M) override {
    ModuleSanitizerCoverage ModuleSancov(Options, Allowlist.get(),
                                         Blocklist.get());
    // Allowlist/Blocklist由参数-fsanitize-coverage-allowlist/-fsanitize-coverage-blocklist指定函数列表,有些场景下会用到
    auto DTCallback = [this](Function &F) -> const DominatorTree * {
      return &this->getAnalysis<DominatorTreeWrapperPass>(F).getDomTree();
    };
    auto PDTCallback = [this](Function &F) -> const PostDominatorTree * {
      return &this->getAnalysis<PostDominatorTreeWrapperPass>(F)
                  .getPostDomTree();
    };
    return ModuleSancov.instrumentModule(M, DTCallback, PDTCallback);
  }
}
```


```cpp
bool ModuleSanitizerCoverage::instrumentModule(
    Module &M, DomTreeCallback DTCallback, PostDomTreeCallback PDTCallback) {
  if (Options.CoverageType == SanitizerCoverageOptions::SCK_None)
    return false;
  if (Allowlist &&
      !Allowlist->inSection("coverage", "src", M.getSourceFileName()))
    return false;
  if (Blocklist &&
      Blocklist->inSection("coverage", "src", M.getSourceFileName()))
    return false;
  C = &(M.getContext());
  DL = &M.getDataLayout();
  CurModule = &M;
  CurModuleUniqueId = getUniqueModuleId(CurModule);
  TargetTriple = Triple(M.getTargetTriple());
  FunctionGuardArray = nullptr;
  Function8bitCounterArray = nullptr;
  FunctionBoolArray = nullptr;
  FunctionPCsArray = nullptr;
  IntptrTy = Type::getIntNTy(*C, DL->getPointerSizeInBits());
  IntptrPtrTy = PointerType::getUnqual(IntptrTy);
  Type *VoidTy = Type::getVoidTy(*C);
  IRBuilder<> IRB(*C);
  Int64PtrTy = PointerType::getUnqual(IRB.getInt64Ty());
  Int32PtrTy = PointerType::getUnqual(IRB.getInt32Ty());
  Int8PtrTy = PointerType::getUnqual(IRB.getInt8Ty());
  Int1PtrTy = PointerType::getUnqual(IRB.getInt1Ty());
  Int64Ty = IRB.getInt64Ty();
  Int32Ty = IRB.getInt32Ty();
  Int16Ty = IRB.getInt16Ty();
  Int8Ty = IRB.getInt8Ty();
  Int1Ty = IRB.getInt1Ty();

  SanCovTracePCIndir =
      M.getOrInsertFunction(SanCovTracePCIndirName, VoidTy, IntptrTy);
  // Make sure smaller parameters are zero-extended to i64 as required by the
  // x86_64 ABI.
  AttributeList SanCovTraceCmpZeroExtAL;
  if (TargetTriple.getArch() == Triple::x86_64) {
    SanCovTraceCmpZeroExtAL =
        SanCovTraceCmpZeroExtAL.addParamAttribute(*C, 0, Attribute::ZExt);
    SanCovTraceCmpZeroExtAL =
        SanCovTraceCmpZeroExtAL.addParamAttribute(*C, 1, Attribute::ZExt);
  }

  SanCovTraceCmpFunction[0] =
      M.getOrInsertFunction(SanCovTraceCmp1, SanCovTraceCmpZeroExtAL, VoidTy,
                            IRB.getInt8Ty(), IRB.getInt8Ty());
  SanCovTraceCmpFunction[1] =
      M.getOrInsertFunction(SanCovTraceCmp2, SanCovTraceCmpZeroExtAL, VoidTy,
                            IRB.getInt16Ty(), IRB.getInt16Ty());
  SanCovTraceCmpFunction[2] =
      M.getOrInsertFunction(SanCovTraceCmp4, SanCovTraceCmpZeroExtAL, VoidTy,
                            IRB.getInt32Ty(), IRB.getInt32Ty());
  SanCovTraceCmpFunction[3] =
      M.getOrInsertFunction(SanCovTraceCmp8, VoidTy, Int64Ty, Int64Ty);

  SanCovTraceConstCmpFunction[0] = M.getOrInsertFunction(
      SanCovTraceConstCmp1, SanCovTraceCmpZeroExtAL, VoidTy, Int8Ty, Int8Ty);
  SanCovTraceConstCmpFunction[1] = M.getOrInsertFunction(
      SanCovTraceConstCmp2, SanCovTraceCmpZeroExtAL, VoidTy, Int16Ty, Int16Ty);
  SanCovTraceConstCmpFunction[2] = M.getOrInsertFunction(
      SanCovTraceConstCmp4, SanCovTraceCmpZeroExtAL, VoidTy, Int32Ty, Int32Ty);
  SanCovTraceConstCmpFunction[3] =
      M.getOrInsertFunction(SanCovTraceConstCmp8, VoidTy, Int64Ty, Int64Ty);

  {
    AttributeList AL;
    if (TargetTriple.getArch() == Triple::x86_64)
      AL = AL.addParamAttribute(*C, 0, Attribute::ZExt);
    SanCovTraceDivFunction[0] =
        M.getOrInsertFunction(SanCovTraceDiv4, AL, VoidTy, IRB.getInt32Ty());
  }
  SanCovTraceDivFunction[1] =
      M.getOrInsertFunction(SanCovTraceDiv8, VoidTy, Int64Ty);
  SanCovTraceGepFunction =
      M.getOrInsertFunction(SanCovTraceGep, VoidTy, IntptrTy);
  SanCovTraceSwitchFunction =
      M.getOrInsertFunction(SanCovTraceSwitchName, VoidTy, Int64Ty, Int64PtrTy);

  Constant *SanCovLowestStackConstant =
      M.getOrInsertGlobal(SanCovLowestStackName, IntptrTy);
  SanCovLowestStack = dyn_cast<GlobalVariable>(SanCovLowestStackConstant);
  if (!SanCovLowestStack) {
    C->emitError(StringRef("'") + SanCovLowestStackName +
                 "' should not be declared by the user");
    return true;
  }
  SanCovLowestStack->setThreadLocalMode(
      GlobalValue::ThreadLocalMode::InitialExecTLSModel);
  if (Options.StackDepth && !SanCovLowestStack->isDeclaration())
    SanCovLowestStack->setInitializer(Constant::getAllOnesValue(IntptrTy));

  SanCovTracePC = M.getOrInsertFunction(SanCovTracePCName, VoidTy);
  SanCovTracePCGuard =
      M.getOrInsertFunction(SanCovTracePCGuardName, VoidTy, Int32PtrTy);

/*

static const char *const SanCovTracePCName = "__sanitizer_cov_trace_pc";
static const char *const SanCovTraceCmp1 = "__sanitizer_cov_trace_cmp1";
static const char *const SanCovTraceCmp2 = "__sanitizer_cov_trace_cmp2";
static const char *const SanCovTraceCmp4 = "__sanitizer_cov_trace_cmp4";
static const char *const SanCovTraceCmp8 = "__sanitizer_cov_trace_cmp8";

*/

// 上述程序用于从 LLVM Context 中获取常见变量类型, 然后根据函数名获取 SanitizerCoverage 
// 内部函数以初始化. 接下来遍历 Module 中所有 Function 以插桩
  for (auto &F : M)
    instrumentFunction(F, DTCallback, PDTCallback);
```

```cpp

void ModuleSanitizerCoverage::instrumentFunction(
    Function &F, DomTreeCallback DTCallback, PostDomTreeCallback PDTCallback) {
  if (F.empty())
    return;
  if (F.getName().find(".module_ctor") != std::string::npos)
    return; // Should not instrument sanitizer init functions.
  if (F.getName().startswith("__sanitizer_"))
    return; // Don't instrument __sanitizer_* callbacks.
  // 省略很多不插桩的逻辑
    
  SmallVector<Instruction *, 8> IndirCalls;
  SmallVector<BasicBlock *, 16> BlocksToInstrument;
  SmallVector<Instruction *, 8> CmpTraceTargets;
  SmallVector<Instruction *, 8> SwitchTraceTargets;
  SmallVector<BinaryOperator *, 8> DivTraceTargets;
  SmallVector<GetElementPtrInst *, 8> GepTraceTargets;
  // 这些变量分别用于不同参数的插桩方法
  // -fsanitize-coverage=trace-pc-guard,indirect-calls,trace-cmp,trace-div,trace-gep
    
  for (auto &BB : F) {  // 遍历当前函数所有BasicBlock代码块
    if (shouldInstrumentBlock(F, &BB, DT, PDT, Options))
      BlocksToInstrument.push_back(&BB);  // 记录所有可以进行插桩的BasicBlock
    for (auto &Inst : BB) {  // 遍历BasicBlock中所有指令
      if (Options.IndirectCalls) {  // 如果启用参数-fsanitize-coverage=indirect-calls
        CallBase *CB = dyn_cast<CallBase>(&Inst);
        if (CB && !CB->getCalledFunction())  // 如果是Call指令,dyn_case会返回非NULL指针
          IndirCalls.push_back(&Inst);  // 记录所有Call指令
      }
      if (Options.TraceCmp) {
        if (ICmpInst *CMP = dyn_cast<ICmpInst>(&Inst))
          if (IsInterestingCmp(CMP, DT, Options))
            CmpTraceTargets.push_back(&Inst);
        if (isa<SwitchInst>(&Inst))
          SwitchTraceTargets.push_back(&Inst);
      }
      if (Options.TraceDiv)
        if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&Inst))
          if (BO->getOpcode() == Instruction::SDiv ||
              BO->getOpcode() == Instruction::UDiv)
            DivTraceTargets.push_back(BO);
      if (Options.TraceGep)
        if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(&Inst))
          GepTraceTargets.push_back(GEP);
      if (Options.StackDepth)
        if (isa<InvokeInst>(Inst) ||
            (isa<CallInst>(Inst) && !isa<IntrinsicInst>(Inst)))
          IsLeafFunc = false;
    }
  }
    
  // 经过多次遍历之后获取到很多BasicBlock和Inst,然后分别使用不同方法进行插桩
  InjectCoverage(F, BlocksToInstrument, IsLeafFunc);
  InjectCoverageForIndirectCalls(F, IndirCalls);
  InjectTraceForCmp(F, CmpTraceTargets);
  InjectTraceForSwitch(F, SwitchTraceTargets);
  InjectTraceForDiv(F, DivTraceTargets);
  InjectTraceForGep(F, GepTraceTargets);
}
```

比如 `InjectCoverage()` 会在前面筛选出的 `BlocksToInstrument` 入口处插入对 `__sanitizer_cov_trace_pc_guard` 的调用.

```cpp
bool ModuleSanitizerCoverage::InjectCoverage(Function &F, ArrayRef<BasicBlock *> AllBlocks,bool IsLeafFunc) {
  if (AllBlocks.empty()) return false;
    CreateFunctionLocalArrays(F, AllBlocks);  // 这里就是创建SantizerCoverage的分支ID记录内存区域
  for (size_t i = 0, N = AllBlocks.size(); i < N; i++)
    InjectCoverageAtBlock(F, *AllBlocks[i], i, IsLeafFunc);  // 遍历所有BasicBlock
  return true;
}

void ModuleSanitizerCoverage::CreateFunctionLocalArrays(
    Function &F, ArrayRef<BasicBlock *> AllBlocks) {
  if (Options.TracePCGuard)
    FunctionGuardArray = CreateFunctionLocalArrayInSection(
        AllBlocks.size(), F, Int32Ty, SanCovGuardsSectionName);  // 记住这个变量,这里的意思是根据当前获取到的所有BasicBlock的数量去创建一个整数数组,用于收集TracePCGuard插桩方法的分支ID记录内存区域
  // 省略其它代码
}

void ModuleSanitizerCoverage::InjectCoverageAtBlock(Function &F, BasicBlock &BB,size_t Idx,bool IsLeafFunc) {
  BasicBlock::iterator IP = BB.getFirstInsertionPt();
  bool IsEntryBB = &BB == &F.getEntryBlock();
  DebugLoc EntryLoc;
  if (IsEntryBB) {
    if (auto SP = F.getSubprogram())
      EntryLoc = DebugLoc::get(SP->getScopeLine(), 0, SP);
    // Keep static allocas and llvm.localescape calls in the entry block.  Even
    // if we aren't splitting the block, it's nice for allocas to be before
    // calls.
    IP = PrepareToSplitEntryBlock(BB, IP);
  } else {
    EntryLoc = IP->getDebugLoc();
  }

  IRBuilder<> IRB(&*IP);  // 前面一通操作是为了获取BasicBlock的第一条指令
  // 省略其它代码
  if (Options.TracePCGuard) {
    auto GuardPtr = IRB.CreateIntToPtr(
        IRB.CreateAdd(IRB.CreatePointerCast(FunctionGuardArray, IntptrTy),
                      ConstantInt::get(IntptrTy, Idx * 4)),
        Int32PtrTy);  // 创建整数指针引用,等价于FunctionGuardArray[Idx]
    IRB.CreateCall(SanCovTracePCGuard, GuardPtr)->setCannotMerge();  // 使用前面创建的引用来创建函数调用,等价于__sanitizer_cov_trace_pc_guard(FunctionGuardArray[Idx]);
  }
  // 省略其它代码
}
```

接下来 Compiler-RT 会将 `__sanitizer_cov_trace_pc_guard_init()` 函数插入程序启动点. 代码位于 `\llvm-preject\compiler-rt\lib\sanitizer_common`

```cpp
  Function *Ctor = nullptr;

  if (FunctionGuardArray)
    Ctor = CreateInitCallsForSections(M, SanCovModuleCtorTracePcGuardName,
                                      SanCovTracePCGuardInitName, Int32PtrTy,
                                      SanCovGuardsSectionName);
```

```cpp
// sanitizer_coverage_fuchsia.cpp

SANITIZER_INTERFACE_WEAK_DEF(void, __sanitizer_cov_trace_pc_guard_init,
                             u32 *start, u32 *end) {  // LLVM默认__sanitizer_cov_trace_pc_guard_init()函数实现代码
  if (start == end || *start)
    return;
  __sancov::pc_guard_controller.InitTracePcGuard(start, end);
}

void InitTracePcGuard(u32 *start, u32 *end) {  // 初始化分支ID内存区域
  if (end > start && *start == 0 && common_flags()->coverage) {
    // Complete the setup before filling in any guards with indices.
    // This avoids the possibility of code called from Setup reentering
    // TracePcGuard.
    u32 idx = Setup(end - start);
    for (u32 *p = start; p < end; ++p) {
      *p = idx++;
    }
  }
}
```