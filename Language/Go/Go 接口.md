### Method

Go 可以为任意变量类型定义方法, 调用方法时, 变量作为参数传入.
```go
type MyStruct struct{
	Value int
}
func (m *MyStruct) ValuePlus(x int)  int{
	m.Value = m.Value + x
}

type MyInt int
func (m MyInt) IsEven() bool {}
```

注意方法可以传入值, 也可传入指针. 传入指针可以在方法内部修改该外部变量.

指针方法, 也可以被值(而非指针)调用, go语法糖:
```go
var myStruct MyStruct
myStruct.ValuePlus(1) // 实际上编译器: (&myStruct).ValuePlus(1)

var myStruct *MyStruct
myStruct.ValuePlus(1) // 推荐这种方法
```

### Interface
定义一组方法的一个签名, 但没有具体实现. 任何拥有这些方法的类型, 都可以抽象为同一类对象, 因为拥有通用*接口*.


不同存储方式有通用读写接口:
```go
type FileStorage interface {
    ReadFile(filename string) ([]byte, error)
    WriteFile(filename string, data []byte) error
}

type LocalDiskStorage struct {
    // ...
}

type CloudStorage struct {
    // ...
}

func ProcessFile(storage FileStorage, filename string) error {
    data, err := storage.ReadFile(filename)
    if err != nil {
        return err
    }
    // Process the data...
    return nil
}
```

不同形状有通用求面积接口:
```go
type Shape interface {
    Area() float64
}

type Rectangle struct {
    Width  float64
    Height float64
}

type Circle struct {
    Radius float64
}

func (r Rectangle) Area() float64 {
    return r.Width * r.Height
}

func (c Circle) Area() float64 {
    return math.Pi * c.Radius * c.Radius
}

```

不同数据库定义通用查询接口:
```go
type Database interface {
    Connect() error
    Query(query string) ([]Row, error)
    // ...
}

type MySQLDatabase struct {
    // ...
}

type PostgreSQLDatabase struct {
    // ...
}

func PerformDatabaseTask(db Database, query string) ([]Row, error) {
    err := db.Connect()
    if err != nil {
        return nil, err
    }
    rows, err := db.Query(query)
    if err != nil {
        return nil, err
    }
    // Process the rows...
    return rows, nil
}
```