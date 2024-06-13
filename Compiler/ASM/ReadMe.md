汇编语言 (Assembly Language) 是机器码指令的助记符, 与机器语言直接对应. 随着*平台, 架构*演进而不断变化.

按指令集架构分类, 可以分为:
- CISC, 复杂指令集, 指令复杂长度不固定, 功能强大. 如 x86.
- RISC, 精简指令集, 指令集精简, 效率高功耗低. 如 ARM, MIPS, RISC-V.

按汇编语法分类, 不同平台和公司的开发者会设计不同的汇编语言语法.

| 架构   | 汇编语言语法 | 平台      | 汇编器                      |
| ------ | ------------ | --------- | --------------------------- |
| x86    | AT&T         | Unix-like | GAS (GNU Assembler)               |
|        | Intel        | Windows   | MASM (Miscrosoft Assembler) |
| ARM    | ARM          |           |                             |

按处理器架构分类, 不同处理器支持功能不同, 汇编语言的指令集也不同.

| 位数 | 架构    | 汇编语法    | 实例处理器                  |
| ---- | ------- | ----------- | --------------------------- |
| 16b  | x86-16  | Intel       | Intel 8086, 80286           |
| 32b  | x86-32  | Intel, AT&T | Intel 80386, 80486, Pentium |
|      | MIPS    |             |                             |
|      | ARM     |             | ARM7, ARM9                  |
| 64b  | x86-64  | Intel, AT&T | AMD64, Intel Core...        |
|      | ARM64   | ARM         | ARM Coretx-A...             |
|      | RISC-V |             |                             |
