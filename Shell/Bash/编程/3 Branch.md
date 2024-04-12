## Functions

bash 中使用函数就像使用其他命令一样.

```bash
function name() {
    shell commands
}
```

Example:

```bash
#!/bin/bash
function hello {
   echo world!
}
hello

function say {
    echo $1
}
say "hello world!"
```

## Conditionals

```bash
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
```

Until 类型

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