> [!cite] 
> [LLVM IR 入门](https://evian-zhang.github.io/llvm-ir-tutorial/01-LLVM%E6%9E%B6%E6%9E%84%E7%AE%80%E4%BB%8B.html)  
> [LLVM Language Reference](https://llvm.org/docs/LangRef.html)  
> [The Architecture of Open Source Applications (Volume 1)LLVM](https://aosabook.org/en/v1/llvm.html)

- **Start Date**: December 2000.
- **Design Principle**: Reusable libraries with clear interfaces, contrasting with the then-prevailing monolithic design of language implementations.
- **Innovation1**: Facilitated the reuse of compiler components across launguages or further tasks (unlike static compilers like GCC which were hard to repurpose for tasks beyond compilation, or scripting languages which had to embed their monolithic runtime into larger applications).
- **Innovation2**: Support both traditional static compiler (like GCC, Free Pascal) and runtime compiler in the form of interpreter (like Python) or Just-In-Time(JIT like .Net, JVM) compiler with more sharings of code.

![](../../attach/Pasted%20image%2020240307150000.png)

LLVM IR 中间代码表示有三种格式:
- `.ll` 文件: 可读 IR, 类似汇编但更易读, 称为 *llvm 汇编码*.
- `.bc` 不可读二进制 IR, 称为*位码 bitcode*. `.ll` 和 `.bc` 其实是等价的, 称为.
- 内存格式,  非持久化保存, 可以加速编译过程.

### Three-Phase Compiler Design

The classical three-phase design of compilers comprised the front end, optimizer, and back end. This design allows for the support of multiple source languages and target architectures by using a common code representation, which enhances portability, broadens the developer base, and fosters community contributions. Open source compilers serving multiple communities tend to produce better optimized code compared to more specialized compilers.

中间代码形式, 使 n 种高级语言和 m 种汇编语言间转换关系从 $n\times m$ 简化为 $n+m$, 让这让编译器更方便地实现跨平台支持.

![](../../attach/SimpleCompiler.png)

实践中这种编译器结构很少被完全实现, 前端后端难分离, 没有标准的中间代码形式, 导致各个语言的代码和优化几乎不能互相复用. 有三种实现该结构的努力:
1. Java and .Net 虚拟机. 提供 JIT 执行程序的 bytecode, [其他语言](http://en.wikipedia.org/wiki/List_of_JVM_languages) 编译为该格式即可被解释执行. 但这种方式必须使用: JIT runtime, garbage collection, particular object model. 这导致了和这种模型差异较大的语言(如C)仅有较低的执行效率.
2. 将源码翻译为C, 然后交给C编译器. 
3. GCC

A second success story is perhaps the most unfortunate, but also most popular way to reuse compiler technology: translate the input source to C code (or some other language) and send it through existing C compilers. This allows reuse of the optimizer and code generator, gives good flexibility, control over the runtime, and is really easy for front-end implementers to understand, implement, and maintain. Unfortunately, doing this prevents efficient implementation of exception handling, provides a poor debugging experience, slows down compilation, and can be problematic for languages that require guaranteed tail calls (or other features not supported by C).

A final successful implementation of this model is GCC[4](https://aosabook.org/en/v1/llvm.html#footnote-4). GCC supports many front ends and back ends, and has an active and broad community of contributors. GCC has a long history of being a C compiler that supports multiple targets with hacky support for a few other languages bolted onto it. As the years go by, the GCC community is slowly evolving a cleaner design. As of GCC 4.4, it has a new representation for the optimizer (known as "GIMPLE Tuples") which is closer to being separate from the front-end representation than before. Also, its Fortran and Ada front ends use a clean AST.

While very successful, these three approaches have strong limitations to what they can be used for, because they are designed as monolithic applications. As one example, it is not realistically possible to embed GCC into other applications, to use GCC as a runtime/JIT compiler, or extract and reuse pieces of GCC without pulling in most of the compiler. People who have wanted to use GCC's C++ front end for documentation generation, code indexing, refactoring, and static analysis tools have had to use GCC as a monolithic application that emits interesting information as XML, or write plugins to inject foreign code into the GCC process.

> There are multiple reasons why pieces of GCC cannot be reused as libraries, including rampant use of global variables, weakly enforced invariants, poorly-designed data structures, sprawling code base, and the use of macros that prevent the codebase from being compiled to support more than one front-end/target pair at a time. The hardest problems to fix, though, are the inherent architectural problems that stem from its early design and age. Specifically, GCC suffers from layering problems and leaky abstractions: the back end walks front-end ASTs to generate debug info, the front ends generate back-end data structures, and the entire compiler depends on global data structures set up by the command line interface.

Pre-LLVM, language implementations rarely shared code, and those that were retargetable were still highly specific to their source language. LLVM's introduction of the Intermediate Representation (IR) changed this by providing a versatile platform for optimization and transformation.

LLVM IR 是独立完备的, 相比之下, GCC 的 GIMPLE 中间表示并不是完备的, 编译器前后端仍需要参考其他信息来完成工作, 这导致基于 GCC 工具链的前后端实现难度较大. 各种代码优化/静态分析/代码重构, 可直接基于 LLVM IR.

![](../../attach/LLVMCompiler1.png)

### Pass

llvm Optimizer 是由不同 Pass 构成的管道, 每个 Pass 读入 llvm IR 后对其进行某种优化. Pass 写在 cpp 文件的匿名 namespace 中, 并提供一个导出函数.

```cpp
// 简单例子
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class MyPass : public PassInfoMixin<MyPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs() << "Function name: " << F.getName() << '\n';
    // 返回一个特殊值，表示未对IR进行修改
    return PreservedAnalyses::all();
  }
};
} // end anonymous namespace

// 导出函数
llvm::PassPluginLibraryInfo getMyPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "MyPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            // 注册 Pass
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "my-pass") {
                    FPM.addPass(MyPass());
                    return true;
                  }
                  return false;
                });
          }};
}

// 使用宏注册插件
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getMyPassPluginInfo();
}
```

llvm 仅链接入所需要的 Pass.o, 无关 Pass.o 不会产生开销. 这也是模块化的优点.

![](../../attach/PassLinkage.png)

![](../../attach/LTO.png)

> Modular vs Monolithic