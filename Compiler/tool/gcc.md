gcc使用格式: `gcc [选项] 要编译的文件 [选项] [目标文件]`

编译过程: 编译 + 链接. 

将 .c 源文件预处理为 .i 文件, 再编译为 .o(.obj) 目标文件, 再链接为可执行程序 .exe.

### 常用

选项:
- `-E` 预处理后即停止.
- `-S` 编译为汇编代码.
- `-c` 汇编为目标代码

- `-static` 静态链接

指令:

`lld`: list dynamic dependencies, 检测可执行程序依赖哪些库.

### 预处理

- 头文件展开
- 宏定义替换
- 去注释
- 执行条件编译

`gcc -E file.c -o file.i` 预处理文件 file.i 仍可读.

### 编译

- 语法分析
- 词法分析
- 语义分析
- 符号汇总

`gcc -S file.i -o file.s` file.s 中是汇编语言.

### 汇编

- 汇编指令转换为二进制指令
- 形成符号表

`gcc -c file.s -o file.o`

### 链接

- 符号表的合并与重定位. 详见 [内存分配](../../System/Memory%20Management/内存分配.md)

`gcc file1.o file2.o file.o -o file.exe`