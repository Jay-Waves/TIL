### 如何写伪代码?

1. 创建$n\times 2$ 表格, 将第一列调窄. 
2. 分别合并一二行左右单元格, 然后去除多余边线, 只保留第一行 第二行和最后一行底线.
3. 自动填写编号: 摁`ctrl+F9`插入域, 输入`seq n \# 0`, 即从0开始递增的域. 然后选中整个表格第一列, 批量复制过去, 然后摁F9更新域. 数字会从1开始自动递增.

### 如何设置多张子图?

0. 打开"表格工具-布局-查看网格线"选项
1. 使用两行表格, 第一行填充图片, 第二行插入小题注. 去除边线
2. 整个表格插入大题注

### 行间公式如何标号?

0. `alt`+`=` 插入行间公式
1. 输入公式后, 在公式最后添加 `#(1)`, 然后回车

### Latex 如何转换为 word/ppt?

使用 pandoc, 实质是先转换为 markdown, 然后转换为 word.
- `pandoc -s main.tex -o main.docx`
- `pandoc -s main.tex -o main.pptx`

