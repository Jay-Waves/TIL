> 调试代码比写代码困难两倍, 写代码用越多奇技淫巧 (自作聪明), 调试代码就越痛苦


### bash 调试参数

```bash
bash -n script # 不执行, 仅检查一遍 syntax errors
bash -v script # 打印每条执行
bash -x script # 打印每条执行的指令, 该指令非原始形式, 而是已经过命令行解析

# use with shebang:
#!/bin/bash -x
```

### 内部调试命令

```bash
	set -u # 检查变量 Use-Before-Init
	set -euo pipefail # 检查管道错误
	# 使用 trap 捕获错误
	trap "echo 'error: Script failed: see failed command above'" ERR 
```

### 捕获信号和返回值

被保留的进程返回值:

| 0     | normal                         |                                    | 错误例子              |
| ----- | ------------------------------ | ---------------------------------- | ----------------- |
| 1     | general erros                  | 除零等致命错误                     | `let "var=1/0"`   |
| 2     | misuse of (bash)shell builtins | 遗漏参数, 权限问题等               | `empty_func() {}` |
| 126   | command invoked not executed   | 不是可执行文件                     | `$ /dev/null`     |
| 127   | command not found              | 一般是 `PATH` 问题找不到可执行文件 | `illegal_cmd`     |
| 128   | invalid argus of exit          | 返回值只能是整数                   | `exit 3.15`       |
| 128+n | fatal error signal n           | `$?` 的返回值                      | `kill -9 $PPID`   |
| 130   | terminated by Ctrl-C           | fatal error signal 2 (130=128+2)   | Ctrl-C            |
| 255   | exit status out of range       | exit 只接收 0-255                  | `exit -1`                  |

其他退出值用户可以自定义, 不过实际标准很混乱. C 为不同错误规定了一些标准返回值, 见 `/usr/include/sysexits.h`

```bash
funciton cleanup {
	echo "exit..."
	exit 1
}
trap cleanup INT
```
