
32 位系统:

```
0xffffffff
	kernel space 1G
0xc0000000
	user space 3G
0x0
```

64 位系统: 高 16 位为 1 时, 代表为内核态程序; 为 0 时, 代表是用户态程序.

```
0xffffffffffffffff
	kernel space 128T
0xffff800000000000
	undefined (canonical address hole)
0x00007ffffffff000
	user space 128T
0x0000000000000000
```

用户空间进程虚拟内存 (32G): 也见 [进程与线程](../Process%20Scheduling/进程与线程.md)

```
0xffff ffff
	kernel space
0xc00j0 0000
	stack:     info about funciton call or local variables. default 8MB.
	( memory waiting for stack )

	memory-mapped file:  dynamic library or shared memory. using `mmap()`
	( memory waiting for file_map )

	( memory waiting for heap )
	heap:      heap, dynamic allocated memory. using `malloc()`
	bss:       uninitialized static or global variables
	data:      initialized static or global variables
	code:      binary executable
0x0804 8000
	reserved
0x0000 0000
```

函数 `malloc()` 在申请空间较小时 (小于 128KB), 使用系统调用 `brk()`, 在**堆段**申请连续内存, 释放时并不立即将内存还给操作系统, 而是缓存起来期再次使用; 在申请空间较大时, 使用系统调用 `mmap()`, 此时在**文件映射段**申请内存, 会触发缺页中断. 缺页中断效率较低, 所以小内存应尽量使用 `brk()` 调用, 但频繁 `brk()` 会产生内存碎片.

64G 虚拟内存分布:

```
0xffff ffff ffff ffff
	kernel space 128T
0xffff 8000 0000 0000
	undefined (canonical address hole)
0x0000 7fff ffff f000
	stack
	(memory waiting for stack)
	
	file map
	(memory waiting for file map)
	
	(memory waiting for heap)
	heap
	bss
	data
	barrier: unable to visit (avoid buffer overflow)
	code
0x0000 0000 0040 0000
	reserved
0x0000 0000 0000 0000
```

可以在 `/proc/pid/maps` 中查看某进程的实际虚拟内存布局.