- 了解 Bash 中的“here documents”，例如 `cat <<EOF ...`。

- 在 Bash 中，同时重定向标准输出和标准错误：`some-command >logfile 2>&1` 或者 `some-command &>logfile`。通常，为了保证命令不会在标准输入里残留一个未关闭的文件句柄捆绑在你当前所在的终端上，在命令后添加 `</dev/null` 是一个好习惯

## `xargs`

`xargs` 可以将字符串或 stdin 转化为命令参数:

```bash
echo "on two three" | xargs mkdir
# ls: ./on  ./two ./three
```

删除 `find` 命令找到的文件:

```bash
find . -name "*.txt" -type f | xargs rm
```

`-n` 指定每次命令执行时使用的参数个数. (输入多个参数时, xargs 实际在并发)

`xargs echo` 拿不准替换结果时, 可以使用先实验一下.

`-I` 指定一个替换字符串:
```bash
# 将 .txt 替换为 .bak
find . -name "*.txt" -type f | xargs -I {} mv {} {}.bak
```

`-0` + `find ... --null`: 当文件名中可能包含空格或特殊字符时, 与 `find ... -print0` 一起使用可以更安全地处理文件名, 多个文件名会被 `NULL` 隔离开而不是空格.

```bash
find . -name "*.txt" -type f -print0 | xargs -0 rm
```

对于循环中文件名中的特殊字符, 可以设置 `IFS='$\n'`. IFS, Internal Field Separator, 是 Bash 用来定义字段或数据项分隔符的特殊变量, 默认值包含 `space, \t, \n`, 在读取一行数据或展开一个数组时可能导致问题.

```bash
IFS=$'\n'
data="one two\nthree four"
echo -e "$data" | while read line; do
  echo "Line: $line"
done
unset IFS
```