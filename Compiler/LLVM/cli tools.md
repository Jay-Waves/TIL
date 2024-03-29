**clang**

llvm 是编译器框架, clang 则可以*控制和驱动*整个编译流程 (调用 llvm 库各种工具, 实现编译). 使用 clang 直接驱动编译过程时, 所有 `IR` 都是内存格式. 在编译器中作为前端. 

btw, gcc 也是驱动 cc, as, ld 程序来编译的. llvm 所有编译工具列表见 [llvm-project source tree](llvm-project%20source%20tree.md)

llvm 工具使用示例:

`clang -S -o3 -emit-llvm main.c -o main.ll`

`clang main.c -o main` -> `./main`

`clang -c -emit-llvm main.c -o main.bc` -> `lli main.bc`

`llvm-dis < main.bc | less`

`llc main.bc -o main.s` -> `gcc main.s -o main.native` -> `./main.native`
