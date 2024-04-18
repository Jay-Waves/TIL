## Functions

bash 中使用函数就像使用其他命令一样. 参数默认是值传递.

```bash
function greet() {
    echo "hello, world"
    return 0
}

greet | cat

# function 输出共有: sdtout, stderr, exit codes.
```

参数传递 != 标准输入

```bash
# 不要搞混 `标准输入` 和 `参数`, 两回事.
function say {
    echo $1
}
say "hello world!"
echo "hello, world!" | say # 这样是错误的, 

function say {
	read input # 用 read 接收标准输入, 或者使用 xargs 将标准输入或字符串转化为参数.
}
```

通过变量名传递间接引用:

```bash
foo() {
	local var=$1
	eval "$var='new value'"
}

var="hello"
foo "var"
echo $var 
>> nwe value

# bash 还呢个通过 ${!var} 来间接引用.
arr["key"]="value"
keyname="key"
echo "${!keyname}
```

## Conditionals

```bash
# 注意 `[` 是参数, 两边必须是空格
if [ expression ]; then
    ...
else
    ...
fi
```

case statements:

```bash
case expression in
    pattern1 )
        statements ;;
    pattern2 )
        statements ;;
    ...
esac
```

Expression Examples:

```bash
statement1 && statement2  # both statements are true
statement1 || statement2  # at least one of the statements is true

str1=str2       # str1 matches str2
str1!=str2      # str1 does not match str2
str1<str2       # str1 is less than str2
str1>str2       # str1 is greater than str2
-n str1         # str1 is not null (has length greater than 0)
-z str1         # str1 is null (has length 0)

-a file         # file exists
-d file         # file exists and is a directory
-e file         # file exists; same -a
-f file         # file exists and is a regular file (i.e., not a directory or other special type of file)
-r file         # you have read permission
-s file         # file exists and is not empty
-w file         # you have write permission
-x file         # you have execute permission on file, or directory search permission if it is a directory
-N file         # file was modified since it was last read
-O file         # you own file
-G file         # file's group ID matches yours (or one of yours, if you are in multiple groups)

file1 -nt file2     # file1 is newer than file2
file1 -ot file2     # file1 is older than file2

-lt     # less than
-le     # less than or equal
-eq     # equal
-ge     # greater than or equal
-gt     # greater than
-ne     # not equal
```

## Loops

for 类型

```bash
for x := 1 to 10 do
begin
  statements
end

# python style
for name [in list]
do
  statements that can use $name
done

# c style
for (( initialisation ; ending condition ; update ))
do
  statements...
done
```

while 类型

```bash
while condition; do
  statements
done

while command; # while 检查命令返回状态, 为 0 (正确退出)时, 执行; 否则退出.
while [ ]; # [ ] 是一个bash命令, 用于判断条件是否成立
```

Until 类型: 相当于 `while !`

```bash
until condition; do
  statements
done
```

### `seq`

生成一个数字序列, 通常用于生成特定循环区间:

```bash
seq First [Step] Last

# 生成 3\n4\n5
seq 3 5

# 生成 3\n5
seq 3 2 5
```