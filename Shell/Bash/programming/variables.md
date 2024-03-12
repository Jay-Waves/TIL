## 获取变量

- 变量引用 `${var}`
- 命令引用 `$(instru)`, 执行时从最内层引用开始执行

## 变量分类

- **环境变量**在任何子程序中都能使用, 有专门的继承内存块
- **自订变量**在当前父程序的子程序中无法使用, 使用`$ export var`命令将其变为环境变量

`$ env` 查看所有环境变量, 系统内部环境变量用*大写字母*

## 设置变量格式

### `#`

`${var#keyword}`从**头**匹配keyword, 符合则删除

`${var##keyword}`从**头***贪婪*匹配keyword, 符合者删除

### `%`

`${var%keyword}` match keywd from the end

`${var%keyword}` match keywd from the end, greedily (as long as possible)

### `/` replace

`${var/old-string/new-string}` replace the old-str with the new one, *only the first one *

`${var//old-str/new-str}` replace *all* the old-str with the new

### test and replace

- `var=${str-expr}` if str *not set (var: str is not existed)*, `var=expr`, otherwise `var=$str` ; `var=${str:-expr}` if str *not set or empty (not existed or be blank str: "")*, `var=expr`, otherwise `var=$str`;
> so sign ':' just marks the differences on the condition that *str is ""*, it can be used in all the following format

- `var=${str+expr}` is the reverse of sign `-`
- `var=${str=expr}` if str is set, var=str, otherwise var=str=expr *var, str both init*
- `var=${str?expr}` if str is not set, *output expr to the stderr*, othewise `var=$str`

## 环境变量

 `env`
 `set`
 
常见环境变量(大写变量):

- PATH 环境变量
- PS1 命令提示符
- $ 当前shell的PID进程代号, 使用`echo ${$}`
- ? 上一个指令所回传的值, 一般正确执行后能返回的值为0, 非0一般就出现了错误捏

> 向PATH添加路径: `PATH="${PATH}:\root"`. 注意为了 **安全** 起见, 不要将相对路径(如`.`)放入PATH中, 因为PATH仅仅按先后顺序扫描寻找命令, 有心人可能会在其他目录放置同名但非法的程序, 管理员可能不小心运行.

### 子程序

子程序仅会继承父程序(bash)的环境变量, 而不会继承自定变量.
除非使用: `export Variable`

### `export`

Displays all environment variables. If you want to get details of a specific variable, use `echo $VARIABLE_NAME`.  

```bash
export
```

Example:

```bash
$ export
AWS_HOME=/Users/adnanadnan/.aws
LANG=en_US.UTF-8
LC_CTYPE=en_US.UTF-8
LESS=-R

$ echo $AWS_HOME
/Users/adnanadnan/.aws
```

这个命令还可以在当前 bash 进程修改变量 `export $var=hello`

直接指定命令环境: `TZ=Pacific/Fiji date`, 获取斐济时间.