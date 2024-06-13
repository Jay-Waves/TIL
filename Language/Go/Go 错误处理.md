Go 语言鼓励显式处理错误, 以提高代码可靠性. 

go 的错误处理基于一个内置接口 `error`. 注意接口 `error` 本身的类型是*接口*, 空值为 `nil`

```go
type error interface {
    Error() string
}
```

go 的标准函数通常会伴随返回一个错误, 通过与 `nil` 比较来检查是否出现了错误:
```go
result, err := SomeFunction()
if err != nil {
    // 处理错误
} else {
    // 处理结果
}
```

使用 `erorrs.New()` 方法可返回一个错误消息.
```go
func Sqrt(f float64) (float64, error) {
    if f < 0 {
        return 0, errors.New("math: square root of negative number")
    }
    // 实现
}
```

自定义错误类型, 可以在接口中附加更多调试信息:
```go
type MyError struct {
    Message string
}

func (e *MyError) Error() string {
    return e.Message
}
```

打印错误信息:
```go
err := fmt.Errorf("An error occurred: %w", originalErr)
```