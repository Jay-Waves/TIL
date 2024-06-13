## 格式

tab 占 8 字符宽度.

```c
switch (suffix) {
case 'G':
case 'g':
        mem <<= 30;
        break;
case 'M':
case 'm':
        mem <<= 20;
        break;
case 'K':
case 'k':
        mem <<= 10;
        /* fall through */
default:
        break;
}
```

行最大长度为 80 字符.

非函数块 (if, switch, for, while do) 的花括号应置于同一行:
```c
if (x == y) {
        ..
} else if (x > y) {
        ...
} else {
        ....
}
```

而函数块花括号应另起一行.
```c
int function(int x)
{
        body of function
}
```

单行块语句不加括号:
```c
if (condition)
        do_this();
else
        do_that();
```

在 `if, switch, case, for, do, while` 关键词后加空格

```c
s = sizeof(struct file)
```

指针星号位置: `char *ptr`

三元操作符和二元操作符两侧加空格
```c
= + - < > * / % | & ^ ?: !=
```

一元操作符两侧不加空格:
```c
& * ++ -- ~ ! sizeof typeof alignof __attribute__ defined . ->
```

宏定义或命名空间结尾, 也要加上名称.
```c
#ifdef CONFIG_STH
...
#endif /* CONFIG_STH */
```

## 风格

函数应简短, 做一件事并做好, 局部变量应为5~10个.

注释不应过分详见, 意图应在代码中显而易见. 即注释应该说明代码**做了什么**, 而不是**怎么做**. 多行注释放在代码块前, 并避免不必要的单行注释 `//` 
```c
/*
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */
```

当有数据结构对其他线程也是可见的, 必须使用**引用计数器**来模拟 GC 行为.

声明堆内存时, 总使用 `sizeof()` 表示大小, 并且不必显式类型转换 `void *`.

```c
p = kmalloc(sizeof(*p), ...);
```

内核不建议多使用 `inline` 关键词, 因为会导致代码体积膨胀. 感觉上去除函数调用开销, 速度加快; 实际, 代码体积膨胀导致*缺页段错误*出现频率更高, 从硬盘读取数据的时间开销可能更大. 

建议超过3行的函数, 不要加 `inline`, 除非明确编译器可以自动优化, 即在编译时是常量, 例如 `kalloc()`. **不要觉得自己比编译器聪明, 很脑死亡.**

函数用返回值定义状态时:
- 对于命令或动作函数, 函数的返回状态应由 error-code 定义 (即 0 代表成功, 非 0 代表各种失败); 
- 对于判断函数, 如 `is_present()`, 应返回布尔值 (即 0 代表失败). 
- 对于返回数值函数, 用超出函数定义范围的值表示失败 (如 `-1, NULL, ERR_PTR` 代表失败)

### 宏

常量或魔术字应使用宏定义, 全大写. 但用宏模拟内联函数时, 不必大写:
```c
// 多行宏, 应被置于 do{}while(0) 块内.
// 避免单行 (if, for) 调用出错, 避免尾随分号
#define macrofun(a, b, c)   \
	do {                    \
		if (a==5)           \
			do_this(b, c);  \
	} while(0)
```

不要使用下列宏用法:
1. 影响控制流的宏
```c
// 这实际影响了该宏调用者的控制流
#define HELL(x)                \
	do {                       \
		if(blah(x) < 0)        \
			return -EBUGGERED  \
	} while (0)
```
2. 使用本地变量的宏, 尤其是常见本地变量 `i,j,k,tmp,val`. 再复杂的宏也不提供本地作用域, 在预处理阶段即被展开.
```c
// tmp 很可能重名
#define FOO(val) bar(tmp, val)
// __foo_tmp 似乎不可能重名
```
3. 将宏用作左值: `FOO(x)=y`, 手贱的人可能将 `FOO()` 改成内联函数形式.
4. 不加括号的宏: 盲目忽略运算优先级和宏替换复杂度
```c
#define CONS 0x4000
#define CONSEXP CONS | 3   // bad
#define CONSEXP (CONS | 3) // good
```

`include/linux/kernel.h` 中定义了很多工具宏, 尽量复用, 不要重复发明创造.

### 命名

- 全局变量命名应是描述性的
- 局部变量命名应尽量简短
- 不要把类型名放进变量名中
- 使用蛇型命名法: `a_name_of_var`
- linux 反对过渡使用 `typedef` 隐藏具体类型, `typedef` 仅用于:
	1. 非透明对象, 用 `typedef` 掩饰其具体类型
	2. 明确数据长度, 如 `u8, u16, u32`, 来避免 `int, long` 架构不一致导致的混淆, 或者对标准库的 `uint32_t` 等进行检查或功能扩充.

