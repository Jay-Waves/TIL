---
date: 2024-04-09
author: Dave Marshall, Jay Waves
---

```c
#include <stdlib.h>

// call unix program in c
int system(char *string);
```

`system` 由 `execl`, `fork`, `wait` 三个 API 实现.

```c
#include <unistd.h>

// execuate and leave, 0 is a NULL terminator
execl(cahr *path, char *arg0, char *agr1, ..., cahr *argn, 0);

int pid; 
pid = fork();
if (pid < 0)
	exit(1);
else if (pid == 0)
	// child preocess
else
	// parent process
	// current pid is its child's pid

// wait() force parent to wait for child. return the pid of child, 
// or -1 for an error. Child's exit status is returned to `status_loc`
int wait( int *status_loc);
```

```c
// terminates and returns exit `status` value. Unix and other C can read status.
// only status of 0 means normal termination.
// std library calls have errors defined in `sys/stat.h`
void exit(int status);
```

例子见 [appendix/fork.c](../../appendix/程序/fork.c), 代码为 K&R 风格.