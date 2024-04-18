查看信息:
- `info skip` 查看 skip 列表
- `info files`
- `info breakpoints`
- `info registers`
- `info args` 当前函数参数
- `info frame`
- `info functions` 查看函数符号
- `info stack`, 类似 `where`
- `info proc status` 
- `info line <n>` 查看某行代码对应的指令地址.
- `show args` 显示命令行参数, `set args`
- `ptype var1` 显示类型定义, 如自定义结构体.

### 字段缩写涵义

- Num: 编号
- Type: 类型, 如 breakpoint
- Disp: 状态
- Enb: 是否可用, 例如当前断点不可用则为 n
- Address: 地址
- What: 在此文件的哪几个函数的第几行