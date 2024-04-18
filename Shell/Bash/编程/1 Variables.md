bash 变量没有严格数据类型, 默认都是字符串类型. 注意整型和布尔型都容易误用.
- 变量引用 `${var}`, 执行变量展开.
- 命令引用 `$(instru)`, 执行命令替换.

## 变量分类

- **环境变量**在任何子程序中都能使用, 有专门的继承内存块
- **自订变量**在当前父程序的子程序中无法使用

常见环境变量: 

- `PATH` 执行环境
- `PS1` 命令提示符
- `BASHPID` 当前进程的 PID, 仅在主脚本中, `$` 和 `BASHPID` 相同.
- `$` 顶层 Bash 脚本的 PID, Bash 中子脚本会直接继承.
- `PPID` 父进程的 PID. Bash 中子脚本会直接继承.
- `?` 上一个指令所传回的错误码
- `!` 上一条命令的PID
- `0` 当前脚本名称
- `1, 2, ..., 9, {10}, {11}` 位置参数, 代表传入的参数
- `#` 传入参数数量
- `IFS` Internal Field Separator, default like: `\s, \t, \n`
- `PWD` 工作目录
- `HOME` 家目录
- `UID, USER, HOSTNAME` 用户ID, 用户名, 主机名
- `RANDOM` 返回一个随机数
- `LINENO` 当前脚本执行到的行数, 常用于调试

### `export`

展示或设置环境变量. 系统级环境变量一般使用*大写字母*.

```bash
$ export
AWS_HOME=/Users/adnanadnan/.aws
LANG=en_US.UTF-8
LC_CTYPE=en_US.UTF-8
LESS=-R

$ echo $AWS_HOME
/Users/adnanadnan/.aws

# 将 TimeZone 设置斐济时间.
$ export $TZ=Pacific/Fiji data 

# 安全原因, 不要将相对路径 (如 `.`) 放入 PATH.
$ export PATH="${PATH}:\root"
```

### `env`

提供对临时环境变量更高级的支持. 

```bash
# 以 VAR1, VAR2 环境变量临时执行某命令
env VAR1=value1 VAR2=value2 command
# 当然这样也可以
VAR1=value1 VAR2=value2 command

# 清除所有环境变量, 包括 export 设置的变量
env -i command 
```


