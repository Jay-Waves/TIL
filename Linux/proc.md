`/proc` 是一种虚拟文件系统, 用于内核态和用户态的通信. 用户可以通过这些文件读取大量系统信息, 并通过修改参数改变系统行为.

- `/proc/cpuinfo` CPU 详细信息
- `/proc/meminfo` 内存使用情况
- `/proc/sys/` 内核各种参数和状态, 用于修改内核行为 (如 IP转发)
- `/proc/partitions` 系统所有分区
- `/proc/mouts` 当前挂载的文件系统
- `/proc/[pid]/cmdline` 启动进程时使用的命令行参数
- `/proc/[pid]/status` 进程状态信息
- `/proc/[pid]/cwd` 进程工作目录
- `/proc/[pid]/exe` 指向该进程所用可执行文件的符号链接
- `/proc/[pid]/fd/` 指向该进程打开的所有文件的符号链接