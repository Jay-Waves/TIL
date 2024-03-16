`Fuzzerriver.cpp/fuzzerDriver()` 函数是 libFuzzer 的入口点.   

```cpp
int FuzzerDriver(int *argc, char ***argv, UserCallback Callback) {
  ...
// 检查用户是否自定义 LLVMFuzzerInitialize() 
// 用户可能在其中定义待测程序的初始化和依赖
  if (EF->LLVMFuzzerInitialize) 
    EF->LLVMFuzzerInitialize(argc, argv);
    ...
   
  unsigned Seed = Flags.seed;
// 外部没有指定随机数种子,那就使用时间戳+pid
  if (Seed == 0)
    Seed = std::chrono::system_clock::now().time_since_epoch().count() + GetPid(); 
// 参数 -v, 输出详细调试信息 
  if (Flags.verbosity) 
    Printf("INFO: Seed: %u\n", Seed);

  Random Rand(Seed); // 随机数生成器
  auto *MD = new MutationDispatcher(Rand, Options); // 数据变异生成器
  auto *Corpus = new InputCorpus(Options.OutputCorpus); // 数据收集器
  auto *F = new Fuzzer(Callback, *Corpus, *MD, Options); // Fuzzer核心

// 创建内存检测线程
// 防止当前进程的内存占用超出阈值.
  StartRssThread(F, Flags.rss_limit_mb); 

// 初始化信号捕获回调函数
  Options.HandleAbrt = Flags.handle_abrt;
  Options.HandleBus = Flags.handle_bus;
  Options.HandleFpe = Flags.handle_fpe;
  Options.HandleIll = Flags.handle_ill;
  Options.HandleInt = Flags.handle_int;
  Options.HandleSegv = Flags.handle_segv;
  Options.HandleTerm = Flags.handle_term;
  Options.HandleXfsz = Flags.handle_xfsz;
  SetSignalHandler(Options); 
    
  ...
  F->Loop(); 
    
  exit(0);
}
```

```cpp
void Fuzzer::Loop() {
...
  while (true) {
...
// 参数-max_total_time, 设定 fuzzer 超时时间
    if (TimedOut()) break; 

    MutateAndTestOne();
  }
...
}

void Fuzzer::MutateAndTestOne() {
  auto &II = Corpus.ChooseUnitToMutate(MD.GetRand());  
  const auto &U = II.U;
  size_t Size = U.size();
// 获取该次测试数据
  memcpy(CurrentUnitData, U.data(), Size); 

...

// 参数 -mutate_depth 控制变异次数, 默认5
  for (int i = 0; i < Options.MutateDepth; i++) { 
    size_t NewSize = 0;
    NewSize = MD.Mutate(CurrentUnitData, Size, CurrentMaxMutationLen);
    Size = NewSize;

// 第一轮变异, hook str(), strcase(), mem() 等函数, 
// 从其参数中获取一些 interesting strings, 用于进一步变异.
    if (i == 0) StartTraceRecording();
    
    II.NumExecutedMutations++;

// Fuzzing, NumFeatures 存储发现的新程序执行路径
    if (size_t NumFeatures = RunOne(CurrentUnitData, Size)) { 
// 如果发现新路径, 就将该数据记录到 Corpus
      Corpus.AddToCorpus( {CurrentUnitData, CurrentUnitData + Size}, NumFeatures, /*MayDeleteFile=*/true);   
// 更新 coverage 数据
      ReportNewCoverage(&II, {CurrentUnitData, CurrentUnitData + Size});
      CheckExitOnSrcPosOrItem();
    }
    StopTraceRecording();
    TryDetectingAMemoryLeak(CurrentUnitData, Size, /*DuringInitialCorpusExecution*/ false);
  }
}

size_t Fuzzer::RunOne(const uint8_t *Data, size_t Size) {
  ExecuteCallback(Data, Size);  // 往下就是调用到LLVMFuzzerTestOneInput()
// 获取当前执行过的代码分支总数
  TPC.UpdateCodeIntensityRecord(TPC.GetCodeIntensity()); 
  size_t NumUpdatesBefore = Corpus.NumFeatureUpdates();
  TPC.CollectFeatures([&](size_t Feature) {
    Corpus.AddFeature(Feature, Size, Options.Shrink);
  });
  size_t NumUpdatesAfter = Corpus.NumFeatureUpdates();

...
// 计算发现了多少新分支路径
  return NumUpdatesAfter - NumUpdatesBefore;  
}


void Fuzzer::ExecuteCallback(const uint8_t *Data, size_t Size) {
...
// 从变异的数据中复制一份到这个内存,后面会用到
  uint8_t *DataCopy = new uint8_t[Size];
  memcpy(DataCopy, Data, Size);  
... 
// 清空所有路径信息
  TPC.ResetMaps();  
// 执行用户自定义的 callback: LLVMFuzzerTestOneInput()
  RunningCB = true;
  int Res = CB(DataCopy, Size);
  RunningCB = false;
...
// 注意, 如果传给 LLVMFuzzerTestOneInput() 的 data 被被测程序修改, 
// libFuzzer 会强制退出!
  if (!LooseMemeq(DataCopy, Data, Size))
    CrashOnOverwrittenData();
  delete[] DataCopy;
}
```

## Coverage

libFuzzer 使用 [Sanitizer Coverage](Coverage.md) 统计覆盖率数据. 在 `FuzzerTracePC.cpp` 中, TracePC(TPC) 类专用于收集 SanCov 收集的插桩信息.

```cpp
ATTRIBUTE_INTERFACE
void __sanitizer_cov_trace_pc_guard_init(uint32_t *Start, uint32_t *Stop) {
  fuzzer::TPC.HandleInit(Start, Stop);
}

ATTRIBUTE_INTERFACE
ATTRIBUTE_NO_SANITIZE_ALL
void __sanitizer_cov_trace_pc_guard(uint32_t *Guard) {
  uintptr_t PC = reinterpret_cast<uintptr_t>(__builtin_return_address(0));
  uint32_t Idx = *Guard;

  getStackDepth();
  fuzzer::codeIntensity++;

  __sancov_trace_pc_pcs[Idx] = PC;
  __sancov_trace_pc_guard_8bit_counters[Idx]++;
}
```

## Mutation

```cpp
MutationDispatcher::MutationDispatcher(Random &Rand,const FuzzingOptions &Options)
    : Rand(Rand), Options(Options) {
// 添加数据变异算法
  DefaultMutators.insert(
      DefaultMutators.begin(),  
      {
          {&MutationDispatcher::Mutate_EraseBytes, "EraseBytes"},
          {&MutationDispatcher::Mutate_InsertByte, "InsertByte"},
          {&MutationDispatcher::Mutate_InsertRepeatedBytes,
           "InsertRepeatedBytes"},
          {&MutationDispatcher::Mutate_ChangeByte, "ChangeByte"},
          {&MutationDispatcher::Mutate_ChangeBit, "ChangeBit"},
          {&MutationDispatcher::Mutate_ShuffleBytes, "ShuffleBytes"},
          {&MutationDispatcher::Mutate_ChangeASCIIInteger, "ChangeASCIIInt"},
          {&MutationDispatcher::Mutate_ChangeBinaryInteger, "ChangeBinInt"},
          {&MutationDispatcher::Mutate_CopyPart, "CopyPart"},
          {&MutationDispatcher::Mutate_CrossOver, "CrossOver"},
          {&MutationDispatcher::Mutate_AddWordFromManualDictionary,
           "ManualDict"},
          {&MutationDispatcher::Mutate_AddWordFromTemporaryAutoDictionary,
           "TempAutoDict"},
          {&MutationDispatcher::Mutate_AddWordFromPersistentAutoDictionary,
           "PersAutoDict"},
      });
  if(Options.UseCmp)
    DefaultMutators.push_back(
        {&MutationDispatcher::Mutate_AddWordFromTORC, "CMP"});

// 注册用户自定义的数据变异方法
  if (EF->LLVMFuzzerCustomMutator)  
    Mutators.push_back({&MutationDispatcher::Mutate_Custom, "Custom"});
  else
    Mutators = DefaultMutators;

// 注册用户自定义的数据杂交方法
  if (EF->LLVMFuzzerCustomCrossOver)
    Mutators.push_back(
        {&MutationDispatcher::Mutate_CustomCrossOver, "CustomCrossOver"});
}
```

![](../../../attach/Pasted%20image%2020240307154740.png)

> source: 
> - [Source-and-Fuzzing](https://github.com/lcatro/Source-and-Fuzzing)
> - https://github.com/Dor1s/libfuzzer-workshop
>

## Q&A



