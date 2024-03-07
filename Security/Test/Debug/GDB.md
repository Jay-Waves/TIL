程序正常编译生成 release 版本, 不能直接用 debugger 调试. debugger 需要编译器生成附带调试信息的 debug 版本, 如命令 `gcc -g`, gcc 将符号表 (程序变量和代码行的内存地址列表) 一并保存在可执行文件中, 方便调试时使用.

发现 bug 后, 改写和编译程序即可, 无需从 gdb 退出. gdb 会自动检测. 

gdb 中的启动信息 (设置的断点, 输入的参数, 监视的变量等), 可以保存为 .gdbinit 文件, 类似 .vimrc 保存启动后自动执行的命令.

指定启动文件: `gdb -command=my_gdb_init my_exe_file`

## 指令

`-tui` : terminal user interface, 基于终端的 GUI 界面.

执行代码:
- `r`: run, 等于 F5
- `c`: continue, 再按一次 F5 
- `until  行号`: 指定位置跳转
- `n`: next, 相当于 F10, 单步跳出
- `s`: step, 相当于 F11, 单步执行
- `finish`: 执行完当前函数

断点
- `b  [源文件:] 函数名/行号`: break, \[指定文件\]打断点
- `info b`  查看断点信息
- `d  断点编号`: delete, 删除一个断点 (必须指定编号)
- `disable b`: 使所有断点无效; `enable b`: 使所有断点有效. 
- `disable b 断点编号`: 禁用某断点
- `break 编号 if 条件`: 创建条件断点. `condition 编号 条件`: 使断点成为条件断点.

显示:
- `l  行号/函数名`: list, 显示 10 行代码. 
- `bt`: 查看函数压栈, 即 backtrace
- `set var`: 修改变量值
- `p  变量名`: print, 打印变量值
- `display`: 跟踪一个变量; `undisplay 变量编号`. 或者 `watch`

可按 Enter 继续查看, gdb 会自动执行上次命令.

### 字段信息

- Num: 编号
- Type: 类型, 如 breakpoint
- Disp: 状态
- Enb: 是否可用, 例如当前断点不可用则为 n
- Address: 地址
- What: 在此文件的哪几个函数的第几行

### 表达式

`(gdb) watch (z > 28)` 设置条件表达式

`(gdb) frame 1` 回溯到之前编号为 1 的栈帧. 当前帧为编号 0, 父帧为编号 1, 依次.

### 定义宏


## 参考

[软件调试艺术](file:///D:/yjw/book/Calibre/Norman%20Matloff/Ruan%20Jian%20Diao%20Shi%20De%20Yi%20Zhu%20(109)/Ruan%20Jian%20Diao%20Shi%20De%20Yi%20Zhu%20-%20Norman%20Matloff.pdf), 教科书. 比较繁琐细节, 没事可以翻出来看一看.