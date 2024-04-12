## Gcov

gcc 自带代码覆盖率指令 `--coverage`, clang 也兼容该操作. 覆盖率数据会被记录到临时文件中, 有命令行工具用于分析这些文件: 
- gcov: gcc 自带工具, 
- llvm-cov: clang 自带工具
- [lcov](https://github.com/linux-test-project/lcov): 上层工具, 用 Perl 实现.
- [gcovr](https://gcovr.com/en/stable): 上层工具, 用 python 实现.

gcov 产生的文件为 `*.gcda` 和 `*.gcno`, `.gcno` 文件为固定源码信息, 如块图信息; `.gcda` 文件为执行时实时覆盖率结果数据, 为每个 obj 文件独立生成. 文件更新为累加性的, 即多次测试的不同覆盖率信息会累加到该文件中, 计算总覆盖率. 

gcov 为每个 obj 文件生成报告数据, 对于有很多 obj 文件的大项目, 收集所有 obj 文件的 `*.gcda` 是困难的, 所以有了 lcov 和 gcovr 的扩展.

**coverage runtime 在程序退出时写入数据 `.gcda`, 如果程序中途异常退出, 则不会有覆盖率数据更新.** 

### colcon coverage + lcov

1. 编译, 此时产生 `*.gcno` 文件

```shell
$ colcon build --mixin coverage
$ find ./build -name "*.gcno"
# a gcno for each source file that was built
$ rm $(find ./build -name "CMake*CompilerId.gcno")
# These are created by the CMake compiler checker
$ mkdir lcov
```

2. (可选) 用 lcov 生成 basline zzero-coverage, 作为参照系

```shell
$ lcov \
    --base-directory .  \ 
    --initial \                           # 生成基准零覆盖率报告
    --capture \                           # 捕获覆盖率数据
    --directory ./build \                 # 在 ./build 目录下查找 .gcno .gcda
    --output-file ./lcov/zero-coverage \     
    --no-external \                       # 不包括外部代码
    --rc lcov_branch_coverage=1           # 分支覆盖率报告
```

3. 执行测试, 或运行程序

```shell
$ colcon test
$ find ./build -name "*.gcda"
# a gcda for each source file whose code was executed
```

4.  用 lcov 生成报告

> lcov 参考[官方仓库](https://github.com/linux-test-project/lcov)

```shell
$ lcov \
    --base-directory . \
    --capture \
    --directory ./build \
    --output-file lcov/coverage \
    --no-external \
    --rc lcov_branch_coverage=1
```

5.  (optional) Combine zero-coverage (from step 2) with coverage output (from step 4)

```shell
$ lcov \
    --add-tracefile lcov/zero-coverage \
    --add-tracefile lcov/coverage \
    --output-file lcov/total_coverage \
    --rc lcov_branch_coverage=1
```

6.  Use `genhtml` to generate browsable coverage information

```shell
$ genhtml \
    --output-directory ./lcov/html \
    "./lcov/total_coverage" \
    --prefix . \
    --legend \
    --demangle-cpp \
    --rc genhtml_branch_coverage=1

# just open is
chromium-browser ./lcov/html/index.html
```

> 参考 [Add a mixin for GCC coverage flags by jpsamper2009 · Pull Request 8 · colconcolcon-mixin-repository](https://github.com/colcon/colcon-mixin-repository/pull/8)

### llvm-cov

`llvm-cov gcov [bin]` 输出代码覆盖率摘要
- `-f` 显示各个函数覆盖情况
- `-b` 显示 branch 选择情况
- `-m` demangle function name. 常搭配 `-f` 一起用, mangle 意思是编译器会将函数名映射为方便机器识别(辨别)的码.
- `-u` 显示非条件性分支, 如 `return`.

### gcovr

```shell
# clang 编译需要使用 llvm-cov 工具
# gcc 则使用 gcov
gcovr --gcov-executable "llvm-cov gcov"

# 输出 json 格式, --json-pretty 更易读, --json-summary 
gcovr --json coverage.json --json-pretty

gcovr --output coverage_summary.json --json-summary-pretty

# 排除不可达代码
gcovr --exclude-unreachable-branches

# 排除分支代码
gcovr --exclude-throw-branches
```

## SanCovr

clang 还有 Google 系列开发的 [sanitizer coverage](https://clang.llvm.org/docs/SanitizerCoverage.html), 和各种 Sanitizer 搭配用于 Fuzzing, 优点是可以统计*边缘覆盖率*. SanCovr 允许向各个程序块插入自定义函数 `__sanitizer_cov_trace_pc_guard()`, 插桩很方便.

![AFL](AFL.md#bitmap)

源码分析见 ![Sanitizer Coverage](Sanitizer%20Coverage.md)