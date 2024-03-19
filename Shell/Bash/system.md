## System Info

### `/proc` Info

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

### `uptime`, `w`

展示已开机时间, 用 `w` 也可, `w` 还可以查看当前登录用户.

### `uname`

Shows kernel information.  

```bash
uname -a # Unix/Kernel 
# or
lsb_release -a # Linux Release 
```

### `sysstat`, `dstat`

`sysstat` 包是收集系统性能和使用情况的工具集, 包括 `mpstat`, `iostat`, `vmstat`, `sar`

`mpstat`, multiprocessor statistics, 报告 CPU 在用户态/内核态/iowait(等待I/O时间)/IDLE(空闲) 状态下所花费时间的百分比.

`iostat`, I/O statistics, 报告 各个设备传输速率, 每次 I/O 操作平均大小, 每秒进行的 I/O 操作数.

`vmstat`, virtual memory statistics, 报告关于虚拟内存/进程/CPU活动等信息.

```shell
$ vmstat 5 # 5s 刷新一次
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 0  0   0  14900552  58160 397884   0    0     7     2    2   10  0  0 100  0  0
 0  0   0  14900552  58160 397884   0    0     0     0    3   36  0  0 100  0  0
```

| r                        | b                        | swpd            | free                 | buff       | cache          |
| ------------------------ | ------------------------ | --------------- | -------------------- | ---------- | -------------- |
| 运行队列                 | 等待 I/O 的进程数        | 虚拟内存量      | 空闲内存量           | 缓冲内存量 | 缓存内存量     |
| si                       | so                       | bi              | bo                   | in         | cs             |
| 从磁盘交换到内存数据量/s | 从内存交换到磁盘数据量/s | 读入块数        | 写出块数             | 中断数/s   | 上下文切换数/s |
| us                       | sy                       | id              | wa                   | st         |                |
| 用户态CPU时间占比        | 系统态CPU时间占比        | 空闲CPU时间占比 | 等待I/O的CPU时间占比 |   被偷取的CPU时间(如虚拟机使用的CPU时间)        |                |

### `dstat`, `glances`

`dstat` 可以视作 `sysstat` 的简易现代替代品. 

更深度系统信息则可以使用 [glances](https://github.com/nicolargo/glances), 会展示更多系统级数据.

### `free`

`free` 检查主存空间

## Debug

### network debug

see [Bash/network](network.md)

### `ldd`

列出程序所依赖的共享库文件 (.so)

<pre>
$ ldd /usr/bin/clang
 linux-vdso.so.1 (0x00007fffc04c3000)
 libclang-cpp.so.14 => /lib/x86_64-linux-gnu/libclang-cpp.so.14 (0x00007f8e58795000)
 libLLVM-14.so.1 => /lib/x86_64-linux-gnu/libLLVM-14.so.1 (0x00007f8e51ec3000)
 libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f8e51c97000)
 libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f8e51bb0000)
 #  [库名] => [地址] (加载到的内存地址)
 </pre>

ldd 存在[严重安全漏洞](https://catonmat.net/ldd-arbitrary-code-execution), 确保目标程序是受信任文件.

### `time`

```
$ time ./lab2
./lab2  0.59s user 0.01s system 19% cpu 3.008 total
```

### `dmesg`

引导及系统错误信息

## Screen

分屏软件: `screen`, `tmux`, `byobu`, `dtach`

## Shutdown

关机命令差别不大:

`shutdown`

`halt` 实际上调用了`shutdown -h`

`poweroff` 常用命令:
- `shutdown -h 30` 30分钟后自动关机
- `shutdown -h now` 立刻关机(root)
- `shutdown -r now` 立刻重启(root)
- `shutdown -r 00:30` 在00:30时候重启(root)
- `init 0` 也可以关机, `init 6`也是重启

重启: `reboot`

#### 关机操作流程

1. 使用`who`查看主机是否有还在线用户
2. 使用`netsat -a`确定是否有网络连接
3. 使用`ps -aux`查看后台进程状态
1. 使用数据同步`sync`
2. 关机
