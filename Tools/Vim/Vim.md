## buffer, window, tab page

```
Vim 会话
	-> 缓冲区, buffer, 代表一个文件, 一个文件可被多个窗口查看.
	-> 窗口, window , 可查看多个文件(缓冲区). 一个窗口仅显示一个缓冲区.
	-> 标签页, tab page, 多个窗口(或标签页, 或分屏)的集合
```

```vim 
$ vim f1 f2   " 激活 f1 buffer, f2 buffer
:e f3         " 激活 f3 buffer, 隐藏 f1,f2 buffer
:ls           " 列出可用 buffer
:bd           " 关闭当前 buffer, 但不关闭 window
```

## navigation, operator, text-objects

**动作** -> 移动光标, 如 `h, j, k, l, w, b`

**操作符, operator** -> 对某区域执行操作, 如 `d, ~, gU, >`

**文本对象操作, text-objects** -> 用于选中区域 (如括号, 单词, 句子), 形式为 `i` (inner), `a`(around) 加上对象标识符. 如 `diw` 删除当前单词, `da(` 删除当前括号及其中内容.

normal mode 下, 应先输入操作符, 再输入动作. 如 `>j`

visual mode 下, 先选中区域, 然后按操作符

`d2a(` 和 `2da(` 等价, `4da(` 和 `2d2a(` 等价.

> `:h navigation`, `:h operator`, `:h text-objects`


## mode

```
normal mode -i,a-> insert mode
insert mode -esc-> normal mode
normal mode -v,V-> visual mode
normal mode -:-> command-line mode
```

****
Vim 总是区分 **行操作** 和 **字符操作**

以行为单位进行操作, 行也基本是最大的操作单位. 更大操作则需要先"选中".

字符操作使用小写字母, 行操作则是对应字符操作的大写字母.

字母g则对应全局操作, 用于命令增强.