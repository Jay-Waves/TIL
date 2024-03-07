`Fuzzerriver.cpp/fuzzerDriver()` 函数是 libFuzzer 的入口点.

```cpp
int FuzzerDriver(int *argc, char ***argv, UserCallback Callback) {
  ...
// 如果用户有自定义LLVMFuzzerInitialize()实现,那么就执行该函数,提供这个函数的作为用户自定义实现接口是因为要对库/程序进行初始化
  if (EF->LLVMFuzzerInitialize) 
    EF->LLVMFuzzerInitialize(argc, argv);
    ...
   
  unsigned Seed = Flags.seed;
// 外部没有指定随机数种子,那就使用时间戳+pid
  if (Seed == 0)
    Seed = std::chrono::system_clock::now().time_since_epoch().count() + GetPid(); 
// 调试输出,参数为-verbosity
  if (Flags.verbosity) 
    Printf("INFO: Seed: %u\n", Seed);

  Random Rand(Seed); // 随机数生成器
  auto *MD = new MutationDispatcher(Rand, Options); // 数据变异生成器
  auto *Corpus = new InputCorpus(Options.OutputCorpus); // 数据收集器
  auto *F = new Fuzzer(Callback, *Corpus, *MD, Options); // Fuzzer核心逻辑模块

// 创建内存检测线程,如果当前进程的内存占用超过阈值之后就退出Fuzzer报告异常
  StartRssThread(F, Flags.rss_limit_mb); 

  Options.HandleAbrt = Flags.handle_abrt;
  Options.HandleBus = Flags.handle_bus;
  Options.HandleFpe = Flags.handle_fpe;
  Options.HandleIll = Flags.handle_ill;
  Options.HandleInt = Flags.handle_int;
  Options.HandleSegv = Flags.handle_segv;
  Options.HandleTerm = Flags.handle_term;
  Options.HandleXfsz = Flags.handle_xfsz;
  SetSignalHandler(Options);  // 初始化信号捕获回调函数
    
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
// 由参数-max_total_time指定的运行时间控制,超时执行就退出
    if (TimedOut()) break; 

    MutateAndTestOne();
  }
  ...
}

void Fuzzer::MutateAndTestOne() {
  auto &II = Corpus.ChooseUnitToMutate(MD.GetRand());  // 从数据收集器中随机挑一个测试数据出来
  const auto &U = II.U;
  size_t Size = U.size();
  memcpy(CurrentUnitData, U.data(), Size);  //  获取测试数据

  // 省略代码
    
  for (int i = 0; i < Options.MutateDepth; i++) {  // 对数据变异多次.由参数-mutate_depth控制,默认值是5
    size_t NewSize = 0;
    NewSize = MD.Mutate(CurrentUnitData, Size, CurrentMaxMutationLen); // 使用前面随机抽取获取到的测试数据作为变异输入生成测试数据
    Size = NewSize;
    if (i == 0)  // 注意,第一次Fuzzing时,会启用数据追踪功能,简而言之就是hook strstr(),strcasestr(),memmem()函数,然后从参数中获取到一些有意思的字符串
      StartTraceRecording();
    II.NumExecutedMutations++;
    if (size_t NumFeatures = RunOne(CurrentUnitData, Size)) {  // 开始Fuzzing,如果使用前面生成的变异数据拿去Fuzzing,发现了新的路径数量,就会保存到NumFeatures,没有发现新路径则NumFeatures=0.
      Corpus.AddToCorpus({CurrentUnitData, CurrentUnitData + Size}, NumFeatures,
                         /*MayDeleteFile=*/true);  // 注意,这一段代码是libFuzzer的核心逻辑之一,如果变异数据发现新路径,那就记录该数据到数据收集器.这是libFuzzer路径探测的核心原理.
      ReportNewCoverage(&II, {CurrentUnitData, CurrentUnitData + Size});
      CheckExitOnSrcPosOrItem();
    }
    StopTraceRecording();
    TryDetectingAMemoryLeak(CurrentUnitData, Size,
                            /*DuringInitialCorpusExecution*/ false);
  }
}

size_t Fuzzer::RunOne(const uint8_t *Data, size_t Size) {
  ExecuteCallback(Data, Size);  // 往下就是调用到LLVMFuzzerTestOneInput()
  TPC.UpdateCodeIntensityRecord(TPC.GetCodeIntensity());  // 获取当前执行过的代码分支总数

  size_t NumUpdatesBefore = Corpus.NumFeatureUpdates();
  TPC.CollectFeatures([&](size_t Feature) {
    Corpus.AddFeature(Feature, Size, Options.Shrink);
  });
  size_t NumUpdatesAfter = Corpus.NumFeatureUpdates();  // 从SanitizerCoverage插桩记录的信息中获取分支数据

  // 省略代码
    
  return NumUpdatesAfter - NumUpdatesBefore;  // 计算发现了多少新分支路径
}


void Fuzzer::ExecuteCallback(const uint8_t *Data, size_t Size) {
  // 省略代码
  uint8_t *DataCopy = new uint8_t[Size];
  memcpy(DataCopy, Data, Size);  // 从变异的数据中复制一份到这个内存,后面会用到
  // 省略代码
  TPC.ResetMaps();  // 清空所有路径信息
  RunningCB = true;
  int Res = CB(DataCopy, Size);  // 执行用户自定义的LLVMFuzzerTestOneInput()
  RunningCB = false;
  // 省略代码
  if (!LooseMemeq(DataCopy, Data, Size))  // 注意这个坑,如果传递给LLVMFuzzerTestOneInput()的data会被程序修改,那么libFuzzer会强制退出
    CrashOnOverwrittenData();
  delete[] DataCopy;
}
```

## Coverage

libFuzzer 使用 [Sanitizer Coverage](../Coverage.md) 统计覆盖率数据. 在 `FuzzerTracePC.cpp` 中, TracePC(TPC) 类专用于收集 SanCov 收集的插桩信息.

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
  DefaultMutators.insert(
      DefaultMutators.begin(),  // 添加数据变异算法
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

  if (EF->LLVMFuzzerCustomMutator)  // 如果存在用户自定义的数据变异方法,那就使用它
    Mutators.push_back({&MutationDispatcher::Mutate_Custom, "Custom"});
  else
    Mutators = DefaultMutators;

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



