## AOT

Ahead-of-time 指程序在执行前就编译为可执行文件. (如 C, C++)

AOT 对编译时间不是很敏感, 可以进行复杂高级的优化算法, 得到效率或 codesize 更优的代码. 但 AOT 无法进行 PGO (profile-guided optimization) 优化

## JIT

Just-in-time(JIT) 在程序运行时编译为可执行文件, 然后运行. JIT 可以接触到运行时信息. (如 C# .Net, Java JVM)

JIT 会调用一个解释器 (Interpreter) 执行字节码, 并收集运行时信息 (如程序热点, 目标平台), JIT 会根据运行时信息进行优化. 比如将字节码中热点代码, 结合目标平台 ABI 或特点, 编译为机器码加速执行.

JIT 在运行时需要先将部分字节码编译为二进制码, 程序存在较长冷启动时间. 并且运行使编译占用资源大 (臭 JVM)

## Interpreter

Interpretation 用函数栈维护线程上下文信息, 将代码一条条转义执行. (如 Python, Perl, JS)

占用内存少 (因为一条一条执行代码), 但执行效率低下.



