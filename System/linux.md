## System Startup 

`init` initialization code from kernel 

## Memory Management

page fault hadling in `mm/memory.c`, memory mapping and page cache in `mm/filema.c`, buffer cache in `mm/buffer.c`, swap cache inn `mm/swap_stat.c, mm/swapfile.c`

## Process Management

scheduler in `kernel/sched.c`, fork in `kernel/fork.c` 

### IPC

`ipc` inter-prcess communication 

### Interrupt Handling

almost platform specific, like `arch/i386/kernel/irq.c` and `include/asm-i386/irq.h`

## PCI

PCI pseudo driver in `drivers/pci/pci.c` and `include/linux/pci.h`

## Device Drivers

- block: like ide.c, device_setup() in `drivers/block/genhd.c`
- char: character-bases devices, like ttys
- cdrom: CDROM
- pci
- scsi
- net
- sound

## File Systems

`fs` file system, like `ext4`, `vfat`

ext2 in `fs/ext2/` and `include/linux/ext2_fs.h, ext2_fs_i.h, ext2_fs_sb.`

VFS in `include/linux/fs.h` and `fs/*`

buffer cache in `fs/buffer.c`

## Network

`net`, `inlucde/net` network

BSD socket in `net/socket.c`

IPv4 in `net/ipv4/af_inet.c`

tcp/ip in `net/ipv4`

drivers in `dirvers/net`

***


`arch` architecture specific code, like `i386`, `x86`, `arm`

`include`, header files



`mm` memory management, architecture specific memory management code lives down `arch/*/mm`



`kernel` main kernel code, like **system call**, architecture specific in `arch/*/kernel`



`lib` library code, architecture specific in `arch/*/lib`

### 系统调用接口

从用户空间到内核的函数调用:
- 实现: `./linux/kernel`
- 依赖于体系结构的部分: `./linux/arch`

### 内存管理

Linux 使用虚拟内存技术, 将内存划分为 4KB 大小的内存页, 并提供寻址技术和交换技术.

实现: `./linux/mm`


### 进程管理

进程是应用程序的一个运行实体, Linux 通过分配时间片"并发"执行进程, 完成进程调度.

Linux 通过优先队列来调度进程.

SCI 提供了 `fork, exec, kill, wait, exit` 等 API, 并提供了信号机制.


### 设备驱动

实际操控操作系统与硬件设备交互, 运行在高特权级的环境中, 驱动错误很可能导致OS崩溃.
