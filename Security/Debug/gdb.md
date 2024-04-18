程序正常编译生成 release 版本, 不能直接用 debugger 调试. debugger 需要编译器生成附带调试信息的 debug 版本, 如命令 `gcc -g`, gcc 将符号表 (程序变量和代码行的内存地址列表) 一并保存在可执行文件中, 方便调试时使用.

发现 bug 后, 改写和编译程序即可, 无需从 gdb 退出. gdb 会自动检测. 

gdb 中的启动信息 (设置的断点, 输入的参数, 监视的变量等), 可以保存为 .gdbinit 文件, 类似 .vimrc 保存启动后自动执行的命令.

指定启动文件: `gdb -command=my_gdb_init my_exe_file`

## 指令

`-tui` : terminal user interface, 基于终端的 GUI 界面.

执行代码:
- `run, r`: 等于 F5
- `continue, c`: 执行到下一个断点, F5
- `until  行号`: 指定位置跳转
- `next, n`: 相当于 F10, 单步跳出
- `step, s`: 相当于 F11, 单步执行
- `skip` 执行 step 时跳过某个函数
- `stepi, si`: 单步执行汇编
- `finish, step out`: 执行完当前函数

断点
- `b  [源文件:] 函数名/行号`: break, \[指定文件\]打断点
- `info b`  查看断点信息
- `d  断点编号`: delete, 删除一个断点 (必须指定编号)
- `disable b`: 使所有断点无效; `enable b`: 使所有断点有效. 
- `disable b 断点编号`: 禁用某断点
- `break 编号 if 条件`: 创建条件断点. `condition 编号 条件`: 使断点成为条件断点.

显示:
- `list, l`: 显示源代码. 
- `backtrace, bt`: 查看函数压栈, 搭配 `frame <id>` 切换栈帧.
- `set var`: 修改变量值
- `display [var]`: 跟踪一个变量, 自动显示信息.
- `watch [var|expr]` 跟踪一个变量, 有变化时中断. 或跟踪表达式, 满足时中断.
- `print [fmt] <expr>` 打印表达式值, 详见 [gdb 查看表达式值](gdb%20查看表达式值.md)
- `info` 查看信息, 见 [gdb 查看调试信息](gdb%20查看调试信息.md)

可按 Enter 继续查看, gdb 会自动执行上次命令.

## 参考

[软件调试艺术](file:///D:/yjw/book/Calibre/Norman%20Matloff/Ruan%20Jian%20Diao%20Shi%20De%20Yi%20Zhu%20(109)/Ruan%20Jian%20Diao%20Shi%20De%20Yi%20Zhu%20-%20Norman%20Matloff.pdf), 教科书. 比较繁琐细节, 没事可以翻出来看一看.
