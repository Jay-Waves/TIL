**clang**

llvm 是编译器框架, clang 则可以*控制和驱动*整个编译流程 (调用 llvm 库各种工具, 实现编译). 使用 clang 直接驱动编译过程时, 所有 `IR` 都是内存格式. 在编译器中作为前端. 

btw, gcc 也是驱动 cc, as, ld 程序来编译的.

`clang -S -emit-llvm main.c`, 输出 `main.ll` 文件.

**opt** 

优化 llvm `IR`, 不改变格式.

**llc**

微观 llvm 编译器. 输出可以是 `s` 或 `o`, 输入是 `IR` (`.ll` 或 `.bc`). 

```ccp
llc -filetype=asm main.bc -o0 -o main.s
llc -filetype=obj main.bc -o0 -o main.o
```

**llvm-mc**

微观 llvm 汇编器, 输入是 `s`, 输出是 `o`. 也可反汇编, 指定参数 `--disassemble`. 核心是输出 obj 文件的库: `MCObjectStreamer`

**lli**

llvm `IR` 的解释器, 也是 JIT 编译器. llvm 可以将 `IR` 解释执行 (用虚拟机运行, 类似 java, 而不是传统 c).

**llvm-link**

llvm `IR` 的链接器, 链接 `IR` 而不是 `obj`.

**llvm-as**

llvm `IR` 的汇编器, 将 `.ll` 汇编为 `.bc`. 如 `llvm-as main.ll`

**llvm-dis**

llvm `IR` 的反汇编器, 将 `.bc` 反汇编为 `.ll`. 如 `llvm-dis main.bc`
