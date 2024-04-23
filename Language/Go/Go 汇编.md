Go 汇编器采用贝尔实验室的 Plan9 汇编语法, 使用**伪寄存器**抽象硬件:
- FP: frame pointer, arguments and locals, like `arg1+8(FP)`
- PC: program counter, jump and branches. similar as `ip` in x86, `rip` in amd64.
- SB: static base pointer, global symbols
- SP: stack pointer, top of stack, like `var+8(SP)`

函数汇编:
```
TEXT pkgname.add(SB), NOSPLIT, $32-16
```
- `TEXT` 指该指令在内存的 `.text` 段
- `pkgname.add` 包名.函数名
- `(SB)` 虚拟 SB 寄存器, 保存基地址. 用 `objdump -j .text -t test | grep 'main.add'` 获取绝对地址
- `NOSPLIT` 指函数内部不进行栈分裂, 等价于 `//go:nosplit`. 当 Go 程序栈大小不够时, 会自动栈分裂, 找一块两倍于当前栈空间的内存. 
- `$32` 指栈帧大小, 不包括返回地址大小 (8bytes), `$16` 指传入的参数和返回值大小.

Go 内置汇编工具:
```bash
go tool compile -N -l -S main.go
```
- `-N` 禁止优化
- `-l` 禁止内联, 等价于函数前加 `//go:noinline`
- `-S` 打印汇编代码

Go 内置反汇编工具:
```bash
go build -gcflags="-N -l" main.go -o test
go tool bojdump mian.o
```

```
<high addr
                           caller
                    +------------------+
                    |                  |
+-----------------> |------------------|
|                   | caller parent BP |
|                   |------------------| <------- BP(pseudo SP)
|                   | local Var0       |
|                   |------------------|
|                   | .....            |
|                   |------------------|
|                   | local VarN       |
|                   |------------------|
|                   | temp unused      |
|                   |------------------|
                    | callee retN      |
caller stack frame  |------------------|
                    | .....            |
|                   |------------------|
|                   | callee ret0      |
|                   |------------------|
|                   | callee argN      |
|                   |------------------|
|                   | .....            |
|                   |------------------|
|                   | callee arg0      |
|                   |------------------| <-------- FP(virtual register)
|                   | return addr      |
+-----------------> |------------------| <----------------+
                    | caller BP        |                  |
BP(pseudo SP)   --->|------------------|                  |
                    | local Var0       |                  |
                    |------------------|                  
                    | ......           |             callee stack frame
                    |------------------|                  
                    | local VarN       |                  |
SP(Real Register) ->|------------------|                  |
                    |                  |                  |
                    |                  |                  |
                    +------------------+ <----------------+
< log addr
```