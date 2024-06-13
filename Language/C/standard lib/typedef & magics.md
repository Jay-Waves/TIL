---
path: <stddef.h>, <stdint.h>, <signal.h>, <errno.h>
date: 24-06-03
---

## stddef.h

```c
typedef unsigned int size_t;
typedef int ptrdiff_t;
typedef unsigned short wchar_t;

#define NULL ((void *)0)
#define offsetof(type, member) ((size_t) &((type *)0)->member)
```

## stdint.h

```c

int8_t
int16_t
int32_t
int64_t

intptr_t
uintptr_t

int_fast8_t

```

## signal.h

```c
typedef int sig_atomic_t;

SIGABRT // 程序异常终止
SIGFPE  // 程序算术运算出错, 如除0或溢出.
SIGILL  // 非法指令
SIGINT  // 中断信号
SIGSEGV // 非法访问 (不存在的) 内存单元
SIGTERM // 请求终止信号

// 捕获信号
void signal(int sig, void (*func)(int));

int raise(int sig);
```

## errono.h

详见 https://en.wikipedia.org/wiki/Errno.h

```c

#define EPERM            1  /* Operation not permitted */
#define ENOENT           2  /* No such file or directory */
#define ESRCH            3  /* No such process */
#define EINTR            4  /* Interrupted system call */
#define EIO              5  /* I/O error */
#define ENXIO            6  /* No such device or address */
#define E2BIG            7  /* Argument list too long */
#define ENOEXEC          8  /* Exec format error */
#define EBADF            9  /* Bad file number */
#define ECHILD          10  /* No child processes */
#define EAGAIN          11  /* Try again */
#define ENOMEM          12  /* Out of memory */
#define EACCES          13  /* Permission denied */
#define EFAULT          14  /* Bad address */
#define ENOTBLK         15  /* Block device required */
#define EBUSY           16  /* Device or resource busy */
#define EEXIST          17  /* File exists */
#define EXDEV           18  /* Cross-device link */
#define ENODEV          19  /* No such device */
#define ENOTDIR         20  /* Not a directory */
#define EISDIR          21  /* Is a directory */
#define EINVAL          22  /* Invalid argument */
#define ENFILE          23  /* File table overflow */
#define EMFILE          24  /* Too many open files */
#define ENOTTY          25  /* Not a typewriter */
#define ETXTBSY         26  /* Text file busy */
#define EFBIG           27  /* File too large */
#define ENOSPC          28  /* No space left on device */
#define ESPIPE          29  /* Illegal seek */
#define EROFS           30  /* Read-only file system */
#define EMLINK          31  /* Too many links */
#define EPIPE           32  /* Broken pipe */
#define EDOM            33  /* Math argument out of domain of func */
#define ERANGE          34  /* Math result not representable */

```

C 语言标准要求的 errno 只有以下三个, 上述大部分都是 GNU 扩展的, linux 中也广泛使用 `linux/errno.h`.

```c
#define EDOM    /* 超出函数定义域范围, 如 sqrt(-1) */
#define ERANGE  /* 结果超出合法范围 */
#define EILSEQ  /* 非法的字节序列 */

int errno;      /* 全局整型变量 */
```