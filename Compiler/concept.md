编译过程:

源程序 --> **预处理器** --> **编译器** --> **汇编器** --> **链接器/加载器**

1. 预处理器, 对源程序进行预处理: 多文件聚合, 宏转写.
2. 编译器, 转换为汇编语言程序.
3. 汇编器, 转换为可重定位的机器代码. 重定位指内存中存放位置可修改: *绝对地址=起始位置+相对地址*
4. 链接器: 修改可重定位地址; 链接器: 参考 [内存管理](../Linux/理论/内存管理.md). 转换为目标机器代码.


编译语言转换：
- 语义转换 Semantic Analysis

### 编译语言

#### 抽象语法树

AST, abtract syntax tree. 将代码分析为树状数据结构.

对于源码 `const a='yujiawei';`

先**词法分析 (Lexical Analysis)**, 使用 scanner, 移除空白符和注释, 将字符流分割为词法单元 (tokens). 如关键词, 标识符, 常量, 运算符等.
```json
[{value: 'const', type: 'keyword'}, {value: 'a', type: 'identifier'}, ...]
```

然后**语法分析 (Syntax Analysis)**, 使用 parser, 将 tokens 列表解析为树状结构. AST 是 CST 的化简版本, 两者语义上等价, 但是 CST 包含更多源码信息.
```json
{
  "type": "VariableDeclaration",
  "start": 482,
  "end": 501,
  "declarations": [
	{
	  "type": "VariableDeclarator",
	  "start": 488,
	  "end": 500,
	  "id": {
		"type": "Identifier",
		"start": 488,
		"end": 489,
		"name": "a"
	  },
	  "init": {
		"type": "Literal",
		"start": 490,
		"end": 500,
		"value": "yujiawei",
		"raw": "'yujiawei'"
	  }
	}
  ],
  "kind": "const"
}
```

最后**语义分析 (Semantic Analysis)**, 进行: 类型检查, 作用域分析, 错误检查与中间代码生成. 词法, 语法, 语义分析通常称为编译器的**前端**.

### 脚本语言

**Interpreter**: 用 parser 编译为 op_code, 然后对 op_code 解析执行.

**GC**: garbage collector, 申请和清除对象内存空间.

**JIT**: Just-in-Time, 为加快脚本执行, 将代码编译为机器代码 (而不是 op_code), 因而也不需要 interpreter 来解析. 