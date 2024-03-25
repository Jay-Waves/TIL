bash 变量无类型.

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

`env`, `set`
 
常见环境变量(大写变量):

- PATH 环境变量
- PS1 命令提示符
- $ 当前shell的PID进程代号, 使用`echo ${$}`
- ? 上一个指令所传回的错误码

> 向PATH添加路径: `PATH="${PATH}:\root"`. 注意为了 **安全** 起见, 不要将相对路径(如`.`)放入PATH中, 因为PATH仅仅按先后顺序扫描寻找命令, 有心人可能会在其他目录放置同名但非法的程序, 管理员可能不小心运行.

### 子程序

子程序仅会继承父程序 (bash) 的环境变量, 而不会继承自定变量.
除非使用: `export Variable`

### `export`

展示或设置环境变量:

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

## 数组

Like other languages bash has also arrays. An array is a variable containing multiple values. There's no maximum limit on the size of array. Arrays in bash are zero based. The first element is indexed with element 0. There are several ways for creating arrays in bash which are given below.

Examples:

```bash
array[0]=val
array[1]=val
array[2]=val
array=([2]=val [0]=val [1]=val)
array=(val val val)
```

To display a value at specific index use following syntax:

```bash
${array[i]}     # where i is the index
```

If no index is supplied, array element 0 is assumed. To find out how many values there are in the array use the following syntax:

```bash
${#array[@]}
```

Bash has also support for the ternary conditions. Check some examples below.

```bash
${varname:-word}    # if varname exists and isn't null, return its value; otherwise return word
${varname:=word}    # if varname exists and isn't null, return its value; otherwise set it word and then return its value
${varname:+word}    # if varname exists and isn't null, return word; otherwise return null
${varname:offset:length}    # performs substring expansion. It returns the substring of $varname starting at offset and up to length characters
```
