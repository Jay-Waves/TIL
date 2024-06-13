---
date: 24-06-03
path: <unistd.h>
---

`unistd.h` 指由 [POSIX API](../../C++/ReadMe.md) 定义的系统调用标准库, 适用于 Unix-like 系统. 在 windows 上头文件则为 `windows.h`

| 分类       | 函数                                         | 作用                           |
| ---------- | -------------------------------------------- | ------------------------------ |
| Process    | `execl(), ...`                               | 执行文件                       |
|            | `fork()`                                     |                                |
|            | `setpgid(), getpgid(), setpgrp(), getpgrp()` | 进程组管理                     |
|            | `getpid(), getppid()`                        |                                |
|            | `getsid(), setsid`                           | session                        |
|            | `nice()`                                     | 获取进程优先级                 |
|            | `sleep()`                                    | 睡眠 (秒)                      |
|            | `exit()`                                     |                                |
| FileSystem | `chdir(), fchdir()`                          | 修改工作目录                   |
|            | `chown(), ...`                               | 修改属主                       |
|            | `close()`                                    | 释放文件描述符                 |
|            | `access()`                                   | 检查用户访问某文件的权限       |
|            | `dup()`                                      | 复制文件描述符                 |
|            | `fsync()`                                    | 同步某文件??                   |
|            | `getcwd()`                                   | 获取当前工作目录               |
|            | `isatty()`                                   | 检查一个文件描述符是否指向终端 |
|            | `link()`                                     | 重命名文件                     |
|            | `lockf()`                                    |                                |
|            | `pipe()`                                     | 创建管道                       |
|            | `pread(), pwrite()`                          | 根据给定偏移量读写文件         |
|            | `read()`                                     | 从文件描述符读文件             |
|            | `readlink()`                                 | 从符号链接读文件               |
|            | `rmdir()`                                    | 删除文件夹                     |
|            | `symlink()`                                  | 创建链接 (文件别名)            |
|            | `sync()`                                     | 将内存中文件系统缓存写入硬盘   |
|            | `ttyname()`                                  |                                |
|            | `write()`                                    | 向一个文件描述符写入           |
|            | `unlink`                                     |                                |
| User/Group | `getgid()`                                   |                                |
|            | `getuid()`                                   |                                |
|            | `getlogin()`                                 | 获取当前用户名                 |
|            | `setuid(), setgid()`                         |                                |


