---
date: 2024-04-09
author: Dave Marshall, Jay Waves
---

```c
#include <stdlib.h>
```

## Random

```c
// get pseudo-random number of range [0, 2^15-1]
int rand(void);
// set seed of pseudo-generator 
void srand(unsigned int seed);

// get non-negative double-precision floating vlues, 
// uniformaly distributed over the interval [0.0, 1.0)
double drand48(void);
double srand48(long seed)
// double erand48(unsigned shor xsubi[3]); // ???

// get non-negative long uniformaly distributed over the interval 
// [0, 2^31)
long lrand48(void);
unsigned short *seed48(unsigned short seed[3]); // set seeds for lrand48

// get signed long over [-2^31, 2^31)
long mrand48(void); 
void lcong48(unsigned short param[7]);// set seed for mcong48(...)
```

使用随机数生成函数之前, 应先设置初始化种子:
```c
// simple examples: use time as seed
srand( (unsigned int) time(NULL) );

ans = rand();
```

## Memory Control

```c
void *malloc(size_t size);
void free(void *ptr);

// alloc num*size chunks of memory, and initilize with NULL
void *calloc(size_t num, size_t size);

// re-alloc memory of previous memory chunk.
void *realloc(void *ptr, size_t new_size);

// user defined wrapper for malloc
void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr==NULL){
		// ...
		exit(EXIT_FAILURE);
	}
	return ptr;
}
```

## Process Control

```c
// Terminates and returns exit `status` value, which can be read by
// Unix or other C. 
// Only status of 0 means normal termination.
// std library calls have errors defined in `sys/stat.h`
void exit(int status);

void abort(void);

// call unix program in c
int system(const char *command);

char *getenv(const char *name);
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

例子见 [appendix/fork.c](../../appendix/程序/fork.c), 代码为 K&R 风格.

## String Conversion

也见 [string](string.md)

```c
double atof(char *str); // str -> floating point value
int atoi(const char *str); // str -> int
int atol(const char *str); // str -> long int
// str -> double-precision floating point number
int strtod(const char *str, char **endptr); 
long strtol(char *str, char *endptr, int radix); 
```

```c
char *str="    123"
char *str1="55.44invalid"
char *str2="invalid123"

i = atoi(str);  // 123
f = atof(str1); // 55.44
i = atoi(str2); // 0
```

- 空格前缀会被跳过
- 后缀中非法字符会被跳过
- 失败返回 0, 同时 `errno=ERANGE`

## Basic Search and Sort

```c
// quick sort
void qsort(void *base, size_t num_elements, size_t element_size, 
					int (*cmp)(void const *, void const *));

// binary search, return NULL if no match found.
void *bsearch(const void *key, const void *base, size_t nel,
					size_t size, int (*cmp)(const void *, const void *));

typedef struct {
	int k;
	// other_data
} Rcd; // record
int cmp(void const *a, void const *b){
	return ((Rcd*)a)->k - ((Rcd*)b)->k);
}
Rcd c = {3, ...};
// even though only c.k field is used, c should be the same type of arr.
ans = bsearch(&c, arr, arrlen, sizeof(Rcd), cmp);
```