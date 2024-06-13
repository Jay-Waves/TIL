Go 语言是编译型面向过程语言, 语法类似C. 为*并发*设计专用语法, 引入 `GC` 和内存安全机制.

注释: `//` 或 `/**/`

### 1 数据类型

1. 布尔值: `true` `false`. *go 语言中只有布尔值可以作为判断条件*
2. 数字类型: 支持复数, 位运算使用补码
3. 字符串: utf-8 标识的 Unicode 文本, 由字节流组成.
4. 派生类型

#### 数字

整型:

| Length | Signed | Unsigned |
| ------ | ------ | -------- |
| 8b     | int8   | uint8    |
| 16b    | int16  | uint16   |
| 32b    | int32  | uint32   |
| 64b    | int64  | uint64   |
| arch   | uint   | int         |

浮点数: IEEE-754, 
- `float32`
- `float64`
- `complex64`
- `complex128`

泛型: 
- `byte` (类似 `int8`)
- `rune` (类似 `int32`)
- `uint`, `int` 使用机器位宽
- `uintptr` (类似 `uint`)


#### 指针

获取地址 `&var`

引用内容 `*var`

空指针为 `nil`

#### 结构体

```go
type StructType struct {
   member definition
   member definition
   ...
   member definition
}
```

声明结构体: 
- `StructType{Member1: <value>, Member2: <value>}`, 可以只初始化一部分.
- `var my_var StructType`
- `new(StructType)` 声明一个新实例, 返回指针.

声明结构体时, gc 会进行*逃逸分析*, 判断: 若变量可能在作用域外访问, 在堆上分配.
```go
// 返回结构体值，此后所有赋值都是值拷贝
var person Person = Person{}
// 返回指针， 而不是值拷贝
var person *Person = &Person{}
```

Go 语言中, 变量和指针都使用 `.` 来访问成员.

#### 变量声明

变量一般声明: `var <id1> [, <id2>] <type>`. 类似的, 也允许同一行多变量声明.

GO 语言会自动初始化变量为对应的**零值**:
- 数值类型 (包括复数) 为 `0`
- 布尔类型为 `false`
- 字符串为 `""`
- 以下几种类型为 `nil`
	- `var a *int`
	- `var a []int`
	- `var a map[string] int`
	- `var a chan int`
	- `var a func(string) int`
	- `var a error // error 是接口`
- 结构 `StructType{}`

隐式声明: `myIntVar := 1`, 等于 `var myIntVar int; myIntVar=1`. 注意 `:=` 符号左侧不应该是已经存在的变量, 而且只能在函数中出现.

类型推理: `var myIntVar = 1`

多变量声明: 常用于全局变量. **只有全局变量可以定义而不使用, 其他会报错.**
```go
var (
    vname1 v_type1
    vname2 v_type2
)
```

常量声明: `const identifier [type] = value`

变量类型转换: `type_name(expression)`

#### 变量作用域

Go 会优先检测局部变量, 再检测全局变量.

### 2 语句

匹配:

```go
switch var1 {
    case val1:
        ...
        fallthrough //fallthrough 强制执行后面一个case
    case val2:
        ...
    default:
        ...
}
```

条件:

```go
if a > b {
	...
}
```

循环: for 有三种写法 

```go
for init; condition; post {} // C for
for condition {} // C while
for {} // loop
```

迭代元素, 如字符串/数组/切片:

```go
for key, value := range oldMap {
    newMap[key] = value
}
```

函数: go 中函数为一等公民 (First Class), 即可以作为参数和变量进行传递.

```go
func MyFunc( [parameter list] ) [return_types | (return_type1, return_type2, ...)] {
   函数体
}
```

### 3 迭代元素

作为形式参数传递时, 栈中元素 (值类型, 固定大小, 如数组和变量) 传递副本, 而堆中元素 (引用类型, 如切片和集合) 会传递指针. 

迭代器会传递拷贝, 但是拷贝仍和原始变量共享底层内存, 拷贝的只是一个信息结构体(数据头, 不是底层数据). 在不同作用域, 如果试图重新分配空间(如 `append`, `make` 等), 不会影响原值.

#### 数组 Array

声明: `var arrayName [size]dataType`

初始化: 
```go
var numbers = [5]int{1, 2, 3, 4, 5}
// same as
numbers := [...]int{1, 2, 3, 4, 5}
```

注意不同大小数组是不同类型, 如 `[5]int` 和 `[10]int` 是不同的类型. 这也导致了 go 标准库函数参数通常是*切片*, 而不是数组. 任意大小数组转化为切片: `array[:]`

#### 切片 Slie

类似动态数组, 即可变长度数组.

**声明方法:**

```go
var s []int // 切片无需长度
s := arr[start:end]
s := make([]int, len, capacity)
s := []array{} // s:= []*Node{&my_node}
```

切片底层是一个结构体:
```go
type slice struct { 
	array unsafe.Pointer // 指向底层数组的指针 
	len int // 切片的长度 
	cap int // 切片的容量 
	}
```
*当使用一个数组的切片时, 如 `array[:]`, 其底层数据仍指向 array*

**内置方法:**
- `len()` 
- `cap()`
- `append()`
- `copy()`


#### 集合 Map

无序默认空值字典.

声明: `map_var := make(map[KeyType]ValueType, initialCapacity)`

方法:
- `delete(map_var, key)`

#### 字符串

`string` 不可变. 性能差, 用于只读文本.

`[]byte` 可变, 存储二进制数据. 在 `capacity` 内操作切片无需内存分配.

组合两个切片:
```go
slice1 := []byte("hello, ")
slice2 := []byte("world!")
result := append(slice1, slice2...)
// ...是展开符号, 将slice2元素全部拆开

var buffer bytes.Buffer
buffer.Write(slice1)
buffer.Write(slice2)
result := buffer.Bytes()
```