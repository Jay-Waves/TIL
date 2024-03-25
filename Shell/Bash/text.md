## Special Formatter

### `pandoc`

[`pandoc`](http://pandoc.org/) 用于 Mardown, HTML 和其他任意格式的转化.

### `jq`, `shyaml`, `xmlstarlet`

[`jq`](http://stedolan.github.io/jq/) 处理 Json; [`shyaml`](https://github.com/0k/shyaml) 处理 YAML; `xmmlstarlet` 处理 XML, 算是上古神器.

### `csvkit`

[`csvkit`](https://github.com/onyxfish/csvkit) 用于处理 Excel 或 CSV 文件, 提供了 `in2csv`，`csvcut`，`csvjoin`，`csvgrep` 等工具

### `hd`, `hexdump`, `xxd`

以十六进制显示二进制文件.

### `bvi`, `hexedit`, `biew`

编辑二进制文件.

### `split`, `csplit`

- 拆分文件可以使用 `split`（按大小拆分）和 `csplit`（按模式拆分）。

### `strings`

从二进制中抽取文本或特定比特, 用于逆向和调试.

```bash
# 提取长度至少为6的字符串, 并输出其字节偏移量(以十六进制)
strings -tx -n 6 --encoding={s,S,b,l,B,L} example.bin
```

## Text Filter

### `awk`

awk is the most useful command for handling text files. It operates on an entire file line by line. By default it uses whitespace to separate the fields. The most common syntax for awk command is

```bash
awk '/search_pattern/ { action_to_take_if_pattern_matches; }' file_to_parse
```

Lets take following file `/etc/passwd`. Here's the sample data that this file contains:

```
root:x:0:0:root:/root:/usr/bin/zsh
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
```

So now lets get only username from this file. Where `-F` specifies that on which base we are going to separate the fields. In our case it's `:`. `{ print $1 }` means print out the first matching field.

```bash
awk -F':' '{ print $1 }' /etc/passwd
```

After running the above command you will get following output.

```
root
daemon
bin
sys
sync
```

For more detail on how to use `awk`, check following [link](https://www.cyberciti.biz/faq/bash-scripting-using-awk).

### `sed`

Stream editor for filtering and transforming text

*example.txt*

```bash
Hello This is a Test 1 2 3 4
```

*replace all spaces with hyphens*

```bash
sed 's/ /-/g' example.txt
```

```bash
Hello-This-is-a-Test-1-2-3-4
```

*replace all digits with "d"*

```bash
sed 's/[0-9]/d/g' example.txt
```

```bash
Hello This is a Test d d d d
```
 
 > [详见](https://www.cnblogs.com/liwei0526vip/p/5644163.html)

### `grep`

```bash
$ grep admin /etc/passwd
_kadmin_admin:*:218:-2:Kerberos Admin Service:/var/empty:/usr/bin/false
_kadmin_changepw:*:219:-2:Kerberos Change Password Service:/var/empty:/usr/bin/false
_krb_kadmin:*:231:-2:Open Directory Kerberos Admin Service:/var/empty:/usr/bin/false
```

force grep to ignore word case by using `-i` option. `-r` can be used to search all files under the specified directory:

```bash
$ grep -r admin /etc/
```

`-F`, `-E` enable functional of regular expr.

`-C [N]` context: print `N` lines after and before matching lines:
```bash
grep -C 5 "pattern"
```

***

## Formatter

### `cut`, `paste`, `join`

Remove sections from each line of files

*example.txt*

```bash
red riding hood went to the park to play
```

*show me columns 2 , 7 , and 9 with a space as a separator*

```bash
cut -d " " -f2,7,9 example.txt
```

```bash
riding park play
```

### `echo` & `printf`

```sh
$ echo -ne "Hello\nWorld\n"
Hello
World
```

`printf` 类似 `echo -n`:

```sh
printf '%s\n' 'hello world'
```

**注意, `echo` 不接受文件输入或标准输入, 如 `echo < hello.txt` 是无效的. 它仅重复输入的参数, 其他情况应使用 `cat`**

### `fmt`

Simple optimal text formatter

*example: example.txt (1 line)*

```bash
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
```

*output the lines of example.txt to 20 character width*

```bash
cat example.txt | fmt -w 20
```

```bash
Lorem ipsum
dolor sit amet,
consetetur
sadipscing elitr,
sed diam nonumy
eirmod tempor
invidunt ut labore
et dolore magna
aliquyam erat, sed
diam voluptua. At
vero eos et
accusam et justo
duo dolores et ea
rebum. Stet clita
kasd gubergren,
no sea takimata
sanctus est Lorem
ipsum dolor sit
amet.
```

### `uniq`, `sort`

Report or omit repeated lines

*example.txt*

```bash
a
a
b
a
b
c
d
c
```

*show only unique lines of example.txt (first you need to sort it, otherwise it won't see the overlap)*

```bash
sort example.txt | uniq
```

```bash
a
b
c
d
```

*show the unique items for each line, and tell me how many instances it found*

```bash
sort example.txt | uniq -c
```

```bash
    3 a
    2 b
    2 c
    1 d
```

### `nl`

number lines, 打印行号, `nl file1`

## Encode

### `iconv`, `uconv`

`iconv` 更改文本编码, `uconv` 则支持一些高级 Unicode 功能.

```sh
# 转换为 Windows 默认的 UTF-16LE 编码 (傻逼巨硬)
iconv -f utf-8 -t utf-16le data.txt
```