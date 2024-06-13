## Go

```bash
go env

$GOPATH
$GOPROXY
```

## 项目组织

使用包:
```go
// 使用 . 可以在程序中直接使用包内函数
import (
	. "fmt"
)

// 定义别名
import (
	fm "fmt"
)

// 给函数，类型等起别名
type MyError = fmt.Error
```

声明命名空间:
```go
package main
```

## 项目依赖管理

```bash
go env
# go module
go mod init MyProject
# 检查依赖, 产生文件 go.mod
go mod tidy
go mod download
# 打印依赖图
go mod graph
```

## 编译器

运行时, 需要同时运行同一空间内所有包:
```shell
go run file1.go file2.go

# 或直接运行当前文件夹下所有包 
go run .
```

go 编译为二进制程序后, 可用 `gdb` 调试. 也能用专用工具 `delve`

```bash
gdb a.out -tui
```

go 内置工具
```bash
go build -n # 仅显示实际编译指令, 不编译

go build -race # 启动内置 thread sanitizer

go tool nm ./a.out # 显示符号信息
go tool objdump    # 反汇编

go vet ./a.out 
```