`/proc` 是一种虚拟文件系统, 用于内核态和用户态的通信. 用户可以通过这些文件读取大量系统信息, 并通过修改参数改变系统行为. `/proc` 并不在磁盘中存在, 而是通过 VFS 在内存中注册自己, 当 VFS 使用 `open(), read()` 等系统调用访问它时, 调用内核信息创建文件.

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
- `/proc/iomem` 物理内存布局
- `/proc/[pid]/maps` 查看虚拟内存布局

### `sysctl` 

`sysctl` 工作在 `/proc/sys` 目录下, 通过读写文件来改变内核参数.

```bash
# 查看所有 sysctl 配置
sysctl -a
# 开启 ip 转发
sysctl -w net.ipv4.ip_forward=1
```

如果要持久化修改配置, 需要修改 `/etc/sysctl.conf`

```bash
sudo echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf
sysctl -p
```