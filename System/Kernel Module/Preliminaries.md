`module_init()` --> `module_exit()`

Kernel modules only use external functions provided by the kernel. Normal library functions run in **user space**, calling system calls in kernel mode to do the real work. use `strace ./hello` to see system calls behind executable `./hello`.

The kernel primarily manages access to resources, be it a video card, hard drive, or memory. 