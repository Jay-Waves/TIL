预处理是[编译器](../../Compiler/Compiler.md)编译的第一个步骤, 所以预处理命令以 `#` 开头.

`#define` 用于定义宏:
```c
#define FALSE 0
#define FALSE TRUE // :(
#define max(A,B) ((A)>(B)?(A):(B)) // 传说中OI高手都是带着一大坨宏编程.
```

宏的匹配规则:
1. 完整标识符匹配替换.
2. 无部分替换, 即宏名作为另一个标识符的一部分时, 不会替换.
3. 字符串和字符常量中的宏名也不会替换.

已存在的宏必须先**取消定义**, 再**定义新值**:
```c
#undefine FALSE
```

`#include` 用于导入其他文件, 即将多个头文件合并到当前文件.
```c
#include <stdio.h> // 标准头文件一般在 /usr/include
#include "file.h"  // 用于导入当前文件夹下的文件
```

`#ifdef ... #elif ... #else`, `#ifndef` 用于条件导入, 常用于处理跨平台依赖和格式兼容, 和防止重复引入头文件.

```c
#ifdef MSVC
	#define LONG_SIZE 32
#else // gcc, clang
	#define LONG_SIZE 64
#endif

#if SYSTEM==MSDOS
	#include <msdos.h>
#else
	#include ...
#endif
```

编译器可以直接定义宏, 但无法覆盖文件内已定义的宏:
```bash
# same as define DEBUG in a.c
cc -DDEBUG a.c -o a.out
```

`#error` 用于生成错误信息

```c
#ifdef MSDOS
	#include <msdos.h>
#elifdef UNIX
	#include <kernel.h>
#else
	#error Wrong OS!
#endif
```

