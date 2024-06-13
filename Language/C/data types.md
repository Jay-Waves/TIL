| types                  | description                                   |
| ---------------------- | --------------------------------------------- |
| `short`                | 2 bytes                                       |
| `int`                  | 4 bytes on x32, x64                           |
| `long`                 | 4 bytes on x32, x64-MSVC; 8 bytes on x64-Unix |
| `long long`            | 8 bytes, after C99                            |
| `char`                 | 1 byte                                        |
| `float`                | 4 bytes IEEE754 floating-point value          |
| `double`               | 8 bytes double-precision floating-point value |
| `size_t`               | store size, platform-independent. `uint` on x32, `ulong(long)` on x64                                               |
| `intptr_t, uintptr_t`  | store pointer                                 |
| `intmax_t, uintmax_t`  |                                               |
| `int8_t, uint8_t, ...` | in `stdlib.h`, platform-independent with fixed width                                 |

https://en.wikipedia.org/wiki/C_data_types#float.h

### bool

C89 标准中没有对 `bool` 的原生支持, C99 才引入了 `stdbool.h` 头文件. 因为 `bool` 极容易和现有 C 程序的宏或定义冲突, 所以委员会使用了保留名称 `_Bool`, 将选择权交给程序员.

```c
#include <stdbool.h>

typdef bool _Bool // 实际上 _Bool 才是 c99 引入的原生布尔类型.
#define true 1
#define false 0
```

在 C99 之前, 开发者已习惯用整数, 宏或枚举模拟布尔值, 所以 `stdbool` 没有被广泛采用.

### Static Variables

静态变量对函数而言是**本地的**, 只会被初始化一次 (函数第一被调用时), 每次函数调用皆访问同一对象, 直到程序结束销毁. 常用于递归.

### Struct 

```c
struct node {
	int key;
	struct node *forward[]; // zero size
}
// using like:
struct node *n = malloc(sizeof(node) + 10*sizeof(struct node*));
```

结构体按顺序从低地址到高地址排列, 但这并不是标准要求而是惯例. 结构体按其内部体积最大成员进行对齐, 结构体内部按各个成员需求进行对齐.

```c
struct data { // total size: 24, 8-byte alignment
	char   a; // offset 0,  1-byte alignment
	int    b; // offset 4,  4-byte alignment
	char   c; // offset 8,  1-byte alignment
	double d; // offset 16, 8-byte alignment
}
```

### Unions

用于表达可能有不同类型的对象. 编译器会预留 Union 中体积最大类型所占的空间.

```c
union number {
	short short_num;
	long long_num;
	double float_num;
} a_num;
```

常规用法是同时封装一个变量用于标识 Union 对象当前的类型.

```c
typedef union {
	jet jetu;
	hellicopter heli;
	cargoplane cargo;
} aircraft;

typedef struct {
	type kind; // type flag
	int speed;
	aircraft descrip;
} an_aircraft;
```

### Enumerated Types

将常量和整数绑定.

```c
// default start with index value 0, increment one by one.
// mon=0, tues=1, and so on.
enum days {mon, tues, ..., sun} week;
enum days week1, week2; // week, week1, week2 are variables
```

使用其他值:
```c
enum escapes { 
	bell='\a',
	backspace='\b',
	tab='\t',
	newline='\n'
	ret='\r'
};
```

使用其他初始值, 后续递增1.

```c
enum months {
	jan=1,
	feb, /*2*/
	mar, /*3*/
	...
	dec, /*12*/
};
```

### Type-Casting

> If in doubt, cast

```c
f = (float) aint / (float) bint;
f = 10.0 / (float) cint;
```