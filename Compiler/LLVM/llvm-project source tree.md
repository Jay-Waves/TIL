`llvm/cmake`: generates system build files

`llvm/examples`

`llvm/include`: llvm 导出的公共头文件库
- `llvm/include/llvm`: all llvm-specific header files 
- `llvm/include/llvm/Support`: generic support libraries, like c++ stl utilities
- `llvm/include/llvm/Config`: header files configured by Cmake

`llvm/lib`: 源代码主体
- `llvm/lib/IR`: core classes like Instruction and BasicBlock
- `llvm/lib/AsmParser`: llvm assembly lanuguage parser lib
- `llvm/lib/Bitcode`: llvm ir bitcode
- `llvm/lib/Analysis`: analyser liek Call Graphs, Induction Variables, Natural Loop Id, etc.
- `llvm/lib/Transforms`: ir-to-ir transformations, such as Inlining, Aggressive Dead Code Elimination, etc.
- `llvm/lib/Target`: describes target architectures, like `Target/X86`
- `llvm/lib/CodeGen`: Code Generator: Instruction Selector, Instructoin Scheduling and Register Allocation.
- `llvm/lib/MC`: process code at machine code level
- `llvm/lib/ExecutionEngine`: directly executes bitcode at runtime as JIT 
- `llvm/lib/Support`:  ...

`llvm/bindings`: llvm 框架的接口和绑定, 用于方便其他非 C 类语言使用 LLVM 架构, 如: Python, Ocaml

`llvm/projects`: ...

`llvm/test` & llvm-test-suite

`llvm/tools`: 命令行工具套件
- bugpoint: 用于定位出错的 Pass
- llvm-ar: archiver
- llvm-as: `.ll` -> `.bc`
- llvm-dis: `.bc` -> `.ll`
- llvm-link: link ir
- lli: llvm JIT  解释器, 直接执行 bitcode
- llc: `.bc` -> `.s`
- opt: IR 优化器/分析器
- llvm-objdump: `.o` -> `.ll`

`llvm/utils`: 写 llvm 的工具


