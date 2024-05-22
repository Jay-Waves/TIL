## 编译过程

源程序 --> **预处理器** --> **编译器** --> **汇编器** --> **链接器/加载器**

### 预处理

预处理 (Prepressing), 主要处理:
1. 将所有定义的宏 `#define` 展开
2. 处理条件预编译指令, 如 `#if, #ifdef`, 详见 [c/preprocessor](../Language/C/c%20preprocessor.md)
3. 将包含的头文件插入到 `#include` 的位置, 该过程递归进行.
4. 删除所有注释 `//`, `/* */`
5. 保留编译器指令 `#pragma`, 因为编译器可能使用.
6. 添加低级调试信息, 如 `#2 "hello.c" 2`. 包括行号和文件名信息.

预处理后, 文件本质还是程序源码. 预处理已有和编译合并的趋势.

### 编译

编译 (Compilation) 过程由预处理好的文件生成汇编代码文件, 包括以下步骤:

```
Source Code 
-- Scanner               --> Tokens
-- Parser                --> Syntax Tree
-- Semantic Analyzer     --> Commented Syntax Tree
-- Source Code Oprimizer --> Intermediate Representation
-- Code Generator        --> Target Code
-- Code Optimizer        --> Fianl Target Code
```

其中词法, 语法, 语义分析通常被称为编译器的**前端**.

#### 词法分析

词法分析 (Lexical Analysis), 使用扫描器, 如 lex, 将字符流分割为词法单元 (tokens); 同时将标识符放入符号表, 将数字及字符串常量放到文字表. 以 `arr[i] = (i + 4) * 6` 为例[^1]:

[^1]: 程序员的自我修养--编译, 装载与库. 俞甲子等. P44

```json
[
	{
		"value": "arr", 
		"type":  "identifier",
	}, {
		"value": "[",
		"type":  "open bracket",
	}, {
		"value": "i",
		"type":  "identifier",
	}, {
		"value": "]",
		"type":  "close bracket",
	}, {
		"value": "=",
		"type":  "assignment",
	}, {
		"value": "(",
		"type":  "open parenthesis",
	}, {
		"value": "i",
		"type":  "assignment",
	...
]
```

#### 语法分析

语法分析 (Syntax Analysis) 调用语法分析器 (Grammar Parser) , 如 yaac (Yet Another Compiler Compiler), 将 tokens 按上下文无关文法解析为树形结构, 称为语法树 (Syntax Tree), 也叫表达式树.

```json
{
	"name": "assign, =",
	"expression": [
		{
			"name": "address, []",
			"expression": [
				{
					"name": "identifier",
					"value": "arr",
				}, {
					"name": "identifier",
					"value": i,
				}
			],
		}, {
			"name": "multply, *",
			"expression": [
				{
					"name": "add, +",
					"expression": [
						{
							"name": "identifier",
							"value": "i",
						}, {
							"name": "number",
							"value": 4,
						},
					]
				}, {
					"name": "number",
					"value": 6,
				}
			]
		},
	],
}
```

#### 语义分析

语义分析 (Semantic Analysis), 调用分析器 (Semantic Analyzer) 完成对静态语义的分析. 主要工作为判断语法合法 (因为通过了语法分析) 的语句是否合法, 如: 声明和类型检查转换, 作用域分析.

```json
{
	"name": "assign, =",
	"type": "integer",
	"expression": [
		{
			"name": "address, []",
			"expression": [
				{
					"name": "identifier",
					"type": "integer[]",
					"value": "arr",
				}, {
					"name": "identifier",
					"type": "integer",
					"value": i,
				}
			],
		}, {
			"name": "multply, *",
			"type": "integer",
			"expression": [
				{
					"name": "add, +",
					"type": "integer",
					"expression": [
						{
							"name": "identifier",
							"type": "integer",
							"value": "i",
						}, {
							"name": "number",
							"type": "integer",
							"value": 4,
						},
					],
				}, {
					"name": "number",
					"type": "integer",
					"value": 6,
				}
			]
		},
	],
	"kind": "mutable",
}
```

#### 中间代码生成与优化

将高级语言转化为中间代码 (intermediate code), 一般跟目标机器与运行时环境无关, 进行进一步优化. 常见的有 三地址码 (Three-address Code), P 码, [LLVM IR](LLVM/llvm%20ir.md).

### 汇编

汇编 (Assembly) 即将汇编代码文件转化为可重定位的目标机器代码, 每个汇编语言基本都对应一条机器指令, 翻译相对简单. 输出一个目标文件 `.o`.

### 链接

链接 (Linking) ...


```
源码 .c       -- 预处理器 -->
源码 .i       -- 编译器/汇编器 -->
目标模块 .o    -- 链接器, 静态链接其他库 .a/.lib -->
装入模块 .exe  -- 装入器, 装入时动态链接其他库 .so/.ddl -->
内存中程序      -- 内存映射, 建立运行时动态链接 -->
```

