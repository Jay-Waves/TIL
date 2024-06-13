## Hello World

```llvm
; main.ll
define i32 @main() {
	return i32 0
}
```

`clang main.ll -o main`

### LLVM IR Layout

- Module
	- Target Information
	- Global Sysmbols
		- **Global Variable**
		- **Funciton Declaration**
		- **Funciton Definition**
	- Other Stuff
- Function 
	- **Argument**
	- Entry Basic Block 
	- **Basic Block**
- Basic Block 
	- Label 
	- **Phi Instruciton**
	- **Instruction**
	- Terminator Instruction

### 编译目标平台信息

![|500](../../attach/Pasted%20image%2020240229161828.png)

目标的数据布局. 具体而言:

- `e`: 小端序
- `m:e`: 符号表中使用ELF格式的命名修饰
- `p270:32:32-p271:32:32-p272:64:64`: 与地址空间有关
- `i64:64`: 将`i64`类型的变量采用64位的ABI对齐
- `f80:128`: 将`long double`类型的变量采用128位的ABI对齐
- `n8:16:32:64`: 目标CPU的原生整型包含8位、16位、32位和64位
- `S128`: 栈以128位自然对齐

`target triple`: [目标平台](../../Language/C/ReadMe.md), 如 `aarc64-apple-darwin`, `x86_64-pc-windows-msvc`


> 参考 [Data Layout](https://llvm.org/docs/LangRef.html#data-layout)

```
+------------------------------+
|          stack_data          |
|         heap_pointer         |  <------------- stack
+------------------------------+
|                              |
|                              |  <------------- available memory space
|                              |
+------------------------------+
| data pointed by heap_pointer |  <------------- heap
+------------------------------|
|          global_data         |  <------------- .data section
+------------------------------+
```

## 数据区与符号表

LLVM IR 全局变量以 `@` 开头, 和函数符号类似. 全局变量实际是指针, 和栈变量类似.

```llvm
@global_var = global i32 0
@global_var = constant i32 0

define i32 @main() {
	ret i32 0
}
```

编译器的符号处理过程:
1. 编译器对于源码中未知函数, 记录其符号; 对于源码中实现的函数, 暴露其符号
2. 链接器收集所有目标文件, 将位置函数符号与其他文件暴露出的符号向匹配, 若成功则称为"成功解析(resolve)了该符号"
3. 动态链接过程也会进行类似的符号解析.

以 C 语言为例, 其主要符号类型有:
```c
int a;                 // 定义在当前文件的全局变量, 别的文件也可以使用.
extrern int b;         // 定义在其他文件的全局变量, 当前文件需要使用该符号.
static int c;          // 定义在当前文件的全局变量, 别的文件不可使用该符号.
void d(void);          // 定义在别的文件的函数, 当前文件需要使用该符号.
void e(void) {}        // 定义在当前文件中的函数, 别的文件可以使用该符号.
static void f(void) {} // 定义在当前文件的函数, 别的文件不可以使用该符号.
```

翻译为对应的 IR:

```llvm
@a = dso_local global i32 0, align 4
; dso_local 明确告知 linker 不许抢占
@b = extrrnal global i32, align 4
@c = internal global i32 0, align 4

declare void @d()

define dso_local void @e() {
	ret void
}

define internal void @f() {
	ret void
}
```

## 寄存器与栈

CPU操作数据时直接操作其内部寄存器, 栈中(或多级 cache 中)数据只是寄存器数量不够时的数据寄存处. LLVM IR 中以 `%` 来标识寄存器, 将所有数据皆抽象在寄存器中, 具体在内存和寄存器中的数据流转管理由 LLVM 去负责. 寄存器可以具名, 也可以是匿名寄存器, 如 `%1` 用数字标识.

```llvm
%local_var = add i32 1, 2
```

在需要**直接操作地址或需要可变变量**时, 也可如下声明栈变量, 此时寄存器中是指针.

```llvm
%local_var = alloca i32
```

上文述栈变量和全局变量皆为指针, 要使用它们, 首先需要 `load` 和 `store` 来和寄存器进行存取数据, 然后再进一步处理:
```llvm
@global_var = global i32 0
%local_var = alloca i32

store i32 1, ptr @global_var

%1 = load i32, ptr @global_var
%2 = add i32 1, %1
```

### SSA

LLVM IR 严格遵循 SSA (Static Single Assignment) 策略, 即*每个寄存器变量在其声明周期内只被赋值一次*. SSA 简化了数据流分析和许多编译器算法, SSA 针对寄存器而不影响操作内存.

```llvm
; 该情况不被允许, 导致 累加 等情况比较棘手
%1 = add i32 1, 2
%1 = add i32 3, 4
```

下文**控制流**一节有更详细叙述.

## 类型系统

官方文档中指出 LLVM 类型有:
- Void Type
- Function Type
- First Class Types
	- Single Value Types
		- Integer Type
		- Floating-Point Types
		- X86_mmx Type
		- Pointer Type
		- Vector Type
	- Label Type
	- Token Type
	- Metadata Type
	- Aggregate Types
		- Array Type
		- Structure Type
		- Opaque Structure Types

汇编语言将数据考虑为二进制序列, 所以是弱类型的. 但 LLVM IR 是**强类型**的, 所有变量都有严格类型, 并且没有隐式类型转换. 数据相关类型如下:

- 空类型 `void`
- 整型 `i1`, `i8`, `i16`, `i32`, `i64` 等. `i1` 用于标识 true/false.
- 浮点型 `float`, `double`

LLVM IR 默认数字有符号, **以补码形式**存储, 但是提供了针对无符号数的操作符 (如 `udiv`, `add`). 是否要将一个变量视为无符号数, 要看具体参与的指令.

LLVM IR 提供了明确的**类型转换指令**:
1. 长整型变为短整型, 直接裁切掉高位: `trunc i32 257 to i8` (将 `100000001` 变为 `00000001`)
2. 短整型变为长整型, 高位零扩展: `zext i8 -1 to i32` (将 `0xff` 变为 `0x000000ff`, 即 255)
3. 短整型变为长整型, 符号扩展: `sext i8 -1 to i32` (将 `0xff` 扩展为 `0xffffffff`, 即 -1)
4. 浮点数和整型间转换, 有: `fptoui .. to`, `uitofp .. to` `sitofp .. to`

### 指针类型

类型 `ptr` 类似 C 语言的 `void *`, 是不包含类型信息的. LLVM 提供指针类型和整型相互转换的指令, 方便灵活操作指针.

```llvm
%x = alloca i32
%addr_x = ptrtoint ptr %x to i64
%also_x = inttoptr i64 %addr_x to ptr
```

### 聚合类型

LLVM IR 也有类似 C 语言的结构体和数组.

```llvm
; 数组
@global_arr = global [4 x i32] [i32 0, i32 1, i32 2, i32 3]

; 字符串初始化, 转义字符(ASCII码)需要两位, 如 \00, 而不是C语言的 \0
@global_str = global [12 x i8] c"Hello world\00"
```

```llvm
; 结构体
@global_struc = global {i32, i8} {i32 1, i8 0}
```

对结构体和数组的字段访问: C语言中 `array` 和 `&array[0]` 意义相同, 但 LLVM 要求必须指明基地址偏移, 将所有指针都看作一个指向数组首地址的指针. (换言之, 将所有 `MyStruct` 指针都视为 `MyStruct[]`, 即使仅有一个元素, 也必须先取 `MyStruct[0]`)

```c
// C
struct MyStruct {
	int x;
	int y;
}
void foo (struct MyStruct *sptr){
	int my_y = sptr[2].y;
}
```

```llvm
; llvm 
%MyStruct type {i32, i32}

define void @foo(ptr %sptr) {
	%my_y_in_stack = alloca i32
	%my_y_ptr = getelementptr %MyStruct, ptr %sptr, i64 2, i32 1
	%my_y_val = load i32, ptr %my_y_ptr
	store i32 %my_y_val, ptr %my_y_in_stack
	ret void
}
```

级联访问:

```llvm
%MyStruct = type {
	i32,
	[5 x i32]
}
%my_struc = alloca [4 x %MyStruct]

; 取 my_struc[2].y[3] 的地址
%1 = getelementptr %MyStruct, ptr %my_struc, i64 2, i32 1, i64 3

; 使用弱类型指针 ptr 是 llvm 12.0 之后引进的特性.
%arr_ptr = alloca [3 x i32]
%2 = getelementptr [3 x i32], [3 x i32]* %arr_ptr, i64 0, i64 0
```

针对寄存器中聚合类型的访问: `getelementptr` 仅针对栈中聚合类型 (即指针指向的聚合类型), 如果聚合类型被读入寄存器中, 则应使用 `extractvalue` 和 `insertvalue` 进行访问:

```llvm
%1 = load %MyStruct, ptr @my_struc 

%2 = extractvalue %MyStruct %1, 1
%3 = insertvalue %MyStruct %1, i32 233, 1
```

## 控制流

高级语言的控制流语句一般有:
```
if .. else
for 
while
switch
```

这些控制流语句实质上都是使用 **跳转** 语句实现的.

### 比较指令

```llvm
%result = icmp uge i32 %a, %b
```

`icmp` 接受的第一个参数 `uge` 称为**比较策略**, 有很多种:
- `eq`, `ne` 相等或不相等
- 无符号比较: `ugt` 大于, `uge` 大于等于, `ult` 小于, `ule` 小于等于
- 有符号比较: `sgt`, `sge`, `slt`, `sle`

### 基本块

LLVM 的函数由一系列基本块 (Basic Block) 组成, 形成一个 CFG (Control Flow Graph). 每个基本块由一个 label 标识, 包含一系列指令, 以及最终的一个终止指令 (如 `ret` 或 `br`)

可以用 opt + graphviz 来生成可视化 CFG:

```shell
opt -p dot-cfg test.ll
dot .main.dot -Tpng -o test.png
```

### for 语句

```c
// in c
for (int i=0; i<4; i++){
	// for body 
}
// next body
```

```llvm
	%i = alloca i32                   ; int i
	store i32 0, ptr %i               ; i=0
	br label %check
check:
	%i_val = load i32, ptr %i
	%result = icmp slt i32 %i_val, 4  ; test if i <4
	br i1 %result, label %for_body, label %next ; 条件跳转
for_body:
	%1 = add i32 %i_val, 1            ; %1 <- i+1, 匿名寄存器 %1.
	store i32 %1, ptr %i              ; i <- %1, 从寄存器取数
	br label %check                   ; 无条件跳转
next:
	ret i32 0
```

### switch 语句

```c
// in c
int x;
switch (x) {
	case 0:
		// case a
		break;
	case 1:
		// case b
		break;
	default:
		// case c
		break;
}
// next
```

```llvm
switch i32 %x, label %C [
	i32 0, label %A
	i32 1, label %B
]
A: 
	; case a
	br label %end
B:
	; case b
	br label %end
C:
	; default case, must provided
	br label %end
end:
	; next
```

### phi 语句

当存在多个控制流路径 (分支, 循环等) 汇合到一个点时, 由于 SSA 策略, 需要一种机制来选择不同路径上的变量值. `phi` 指令可以根据控制流来向(上一个被执行的BB), 选择对应的值.

```c
int factorial(int val){
	int temp = 1;
	for (int i=3; i<=val; ++i)
		temp *= i;
	return temp;
}
```

错误的 llvm ir 示范如下. `%i = add i32 %i, 1` 这样是违反 SSA 策略的, `%i` 在 LLVM 中被抽象为**寄存器**, CPU中的寄存器显然不能用自己来更新自己, 前后两个 `%i` 的作用域重叠了.

```llvm
define i32 @factorial(i32 %val){
entry:
	%i = add i32 0, 2
	%temp = add i32 0, 1
	br label %check_for_condition
check_for_condition:
	%i_leq_val = icmp sle i32 %i, %val
	br i1 %i_leq_val, label %for_body, label %end_loop
for_body:
	%temp = mul i32 %temp, %i   ; 错误!! 声明周期内寄存器多次赋值, 违反SSA
	%i = add i32 %i, 1          ; 错误!!
	br label %check_for_condition
end_loop:
	ret i32 %temp
}
```

使用 phi 指令, 检测数据来向如下. 此时虽然 `%new_temp` 仍被多次声明, 但每次声明都是不同实例, 所以仍遵循 SSA 策略.
```llvm
define i32 @factorial(i32 %val){
entry:
	br label %check_for_condition
check_for_condition:
	%current_i = phi i32 [2, %entry], [%i_plus_one, %for_body]
	%temp = phi i32 [1, %entry], [%new_temp, %for_body]
	%i_leq_val = icmp sle i32 %current_i, %val 
	br i1 %i_leq_val, lebel %for_body, label %end_loop 
for_body:
	%new_temp = mul i32 %temp, %current_i
	%i_plus_one = add i32 %current_i, 1
	br label %check_for_condition 
end_loop:
	ret i32 %temp
}
```

使用栈变量(内存)也可以绕过 SSA, 但是程序会更加冗长:
```llvm
define i32 @factorial(i32 %val){
entry:
	%i.addr = alloca i32
	%temp.addr = alloca i32
	store i32 2, ptr %i.addr 
	store i32 1, ptr %temp.addr ; 或 store i32 1, i32* %temp.addr
	br label %for.check 
for.check:
	%current_i = load i32, ptr %i.addr 
	%temp = load i32, ptr %temp.addr 
	%i_leq_val = icmp sle i32 %current_i, %val 
	br i1 %i_leq_val, label %for.body, label %for.end  
for.body:
	%i_plus_one = add i32 %current_i, 1
	%new_temp = mul i32 %temp, %current_i
	store i32 %i_plus_one, ptr %i.addr 
	store i32 %new_temp, ptr %temp.addr 
	br label %for.check  
for.end:
	ret i32 %temp
}
```

### select 语句

`select` 语句和 `phi` 类似, 但是其仅适用于**二选一**的情况.

```c
// in c
void foo(int x){
	int y;
	if (x > 0){
		y = 1;
	} else {
		y = 2;
	}
}
```

不适用 `select` 时, 需要借助栈变量. 代码冗长, 且访问内存速度要慢于寄存器速度.

```llvm
; not use select
define void @foo(i32 %x){
	%y = alloca i32
	%1 = icmp sgt i32 %x, 0
	br i1 %1, label %btrue, label %bfalse
btrue:
	store i32 1, ptr %y
	br label %end
bfalse:
	store i32 2, ptr %y
	br label %end
end:
	ret void
}
```

使用 `select` 类似 python/rust 中的单行表达式: `let y = if x>0 {1} else {2}`

```llvm
; use select
define void @foo(i32 %x){
	%1 = icmp sgt i32 %x, 0
	%y = select i1 %1, i32 1, i32 2
	ret void
}
```

**注意下面这种写法是违反SSA策略的, 很错误**. 在不同代码路径中 `%y` 可能有不同的值, 不利于编译器对程序数据流进行分析. SSA 要求每个变量在作用域内的值是唯一确定的 , 即使在复杂控制流下也是单一赋值的 (注意更新这个变量不会违反SSA, 因为实际上是创建了一个新变量)

```llvm
define void @foo(i32 %x){
	%1 = icmp sgt i32 %x, 0
	br i1 %1, label %btrue, label %bfalse
btrue:
	%y = add i32 0, 1
	br label %end
bfalse:
	%y = add i32 0, 2
	br label %end
end:
	ret void
}
```

## 函数

函数和全局变量都会被记录进符号表.

```llvm
define dso_local void @foo() #0 {
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

; #0 称为属性组, 可以代指定义的一长串属性. 属性直接写在函数后也是可行的, 比如:
define dso_local void @foo() noinline nounwind {
	ret void
}
```

```c
// in c
void foo() {}
```

需要使用其他模块的函数时, 需要在本模块先使用 `declare` 声明该函数:

```llvm
declare i32 @printf(i8*, ...) #1
```

LLVM IR 调用函数的方式和高级语言类似:

```llvm
define void @bar() {
	%1 = call i32 @foo(i32 1)
}
```

### 内置函数

LLVM IR 提供了一些内置的基础函数实现, 如 `llvm.memcpy`

