### 空格

latex多个连续空格会被视为一个, 如下命令可输入多空格:
- `\ ` 用反斜杠转义
- `\quad` 产生一个em宽度空格
- `\hspace{1cm}` 产生特定长度空格
- `\phatom{text}` 产生和text宽度同的空白空间, 但不显示文本
- `\hfill` 类似qt的左strech, 将文本挤到右侧

### 换行

段内换行: markdown 在行末加两个空格, latex 则加 `\\`. 否则编译后不换行.

换段: 一个空行, 或 `\par`

## 公式

### 如何放大公式?

不同程度: `\large` `\Large` `\LARGE` `\huge` `\Huge`

作为修饰符, 修饰整个作用域: `{\large x}`

### 如何让角标显示在正上下方?

$\sum_{k=1}$, $\sum\limits_{k=1}$