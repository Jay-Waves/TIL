> [!cite] 
> [LLVM IR 入门](https://evian-zhang.github.io/llvm-ir-tutorial/01-LLVM%E6%9E%B6%E6%9E%84%E7%AE%80%E4%BB%8B.html)  
> [LLVM Language Reference](https://llvm.org/docs/LangRef.html)

![](../../attach/Pasted%20image%2020240307150000.png)

**编译过程:**  
`Source Code` --> Clang (frontend)  
--> `IR` --> LLVM IR Linker, LLVM Optimizer  
--> `IR` --> LLVM Compiler (backend)  
--> `Assembly` --> Assembler  
--> `Object` --> Linker  
--> `Executable`

**优化:**  
`IR` --> Opt --> `IR`

llvm 在常见编译过程文件中间, 插入了 `IR` 中间文件用于额外*优化和混淆*. `IR` 有三种:
- `.ll` 文件: 可读 IR, 类似汇编但更易读, 称为 *llvm 汇编码*.
- `.bc` 不可读二进制 IR, 称为*位码 bitcode*. `.ll` 和 `.bc` 其实是等价的, 称为.
- 内存格式,  非持久化保存, 可以加速编译过程.

**通过使用 IR 这种中间代码形式, n种高级语言和m种汇编语言间转换关系从 $n\times m$ 简化为 $n+m$, 让新兴语言 (如 rust, swift) 更方便地实现跨平台支持. 本质上, [LLVM IR](IR.md) 仍是接近汇编的一种语言.** 其中, 平台指 `CPU Arch - Vendor - OS`, 如 `x86_64-unknown-linux-gnu` / `aarch64-apple-darwin` / `x86_64-pc-windows-msvc`. 顺带一提, IR 中指定平台的语句是: `target triple = "amd64-unknown-linux-gnu"`

### 工具

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
