
| 功能             | pdb（Python）                      | gdb（GNU）                         | lldb（LLVM）                                             |
| ---------------- | ---------------------------------- | ---------------------------------- | -------------------------------------------------------- |
| 启动程序         | python -m pdb script.py            | gdb ./executable                   | lldb ./executable                                        |
| 设置断点         | b [行号/文件名:行号]               | b [行号/函数名]                    | b [行号/函数名]                                          |
| 列出源代码       | l                                  | list                               | l / list                                                 |
| 单步执行         | n (next)                           | n (next)                           | n (next) / thread step-over                              |
| 进入函数         | s (step)                           | s (step)                           | s (step in) / thread step-in                             |
| 执行直到返回     | r (return)                         | finish                             | finish / thread step-out                                 |
| 打印变量值       | p [变量名]                         | print [变量名]                     | p / frame variable [变量名]                              |
| 查看当前执行位置 | l (list)                           | info line                          | frame info                                               |
| 继续执行         | c (continue)                       | c (continue)                       | c (continue)                                             |
| 查看调用栈       | where / bt                         | bt (backtrace)                     | bt (backtrace)                                           |
| 切换栈帧         | `frame <n>`                        | `frame <n>`                        | `frame select <n>`                                                         |
| 切换堆栈帧       | up/down                            | up/down                            | up/down                                                  |
| 查看所有局部变量 | locals()                           | info locals                        | frame variable                                           |
| 设置条件断点     | b [行号] if [条件]                 | b [行号/函数名] if [条件]          | b [行号/函数名] -c [条件]                                |
| 查看所有断点     | b                                  | info breakpoints                   | breakpoint list                                          |
| 删除断点         | cl [断点号]                        | delete [断点号]                    | breakpoint delete [断点号]                               |
| 禁用/启用断点    | disable [断点号] / enable [断点号] | disable [断点号] / enable [断点号] | breakpoint disable [断点号] / breakpoint enable [断点号] |
| 查看变量类型     | N/A                                | ptype [变量名]                     | frame variable --show-types [变量名]                     |
| 查看全局变量     | N/A                                | info variables                     | target variable                                          |
| 修改变量的值     | N/A                                | set var [变量名]=[值]              | expr [变量名] = [值]                                     |
| 退出调试器       | q (quit)                           | q (quit)                           | q (quit) / exit                                          |

#python