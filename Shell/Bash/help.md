### `man`

文档手册 (manual), 难懂但是全. 类似还有 `help, info, [-h], [--help]`

```bash
man command

man -k # 等价于 apropos, 进行关键词搜索
```

```bash
# 查看 ASCII 表
man ascii
# 编码信息
man unicode 
man utf-8
man latin1
```

### `type`

`type command` 显示某个命令的类型, linux中命令有四种类型:
- /bin文件夹中的可执行程序
- shell builtins内置程序
- shell function即scripts
- alias 别名

<pre>
$ type cd
cd is a shell builtin
</pre>

### `whatis`

显示单行, 非常简短的命令介绍

<pre>
$ whatis bash
bash (1)             - GNU Bourne-Again SHell
</pre>

### `whereis`

whereis searches for executables, source files, and manual pages using a database built by system automatically.

Example:

```bash
$ whereis php
/usr/bin/php
```

### `which`

模拟搜索 PATH 查询可执行文件的过程, 若成功, 显示完整路径. (不能检查其他类型命令!) 可用于检查当多个重名程序存在于 PATH 中时, bash 究竟执行了哪一个.

```bash
$ which php
/c/xampp/php/php
```

### [128K限制](https://wiki.debian.org/CommonErrorMessages/ArgumentListTooLong)

通配符匹配大量文件名时, 遇到 "Argument list too long" 的错误信息.

比如直接重定向会报错:
```bash
find . type f -name "*.txt" | grep "pattern"
```

可以使用 `xargs` 或 `find -exec` 解决:

```bash
find . -type f -name "*.txt" -exec grep "pattern" {} +

find . -type f -name "*.txt" -print0 | xargs -0 grep "pattern"
```

### 更多资源

- [awesome-shell](https://github.com/alebcay/awesome-shell) 一份工具列表.
- [awesome-osx-command-line](https://github.com/herrbischoff/awesome-osx-command-line): 一份针对 osX 的工具列表
- [Strict mode](http://redsymbol.net/articles/unofficial-bash-strict-mode/): bash 编程指导
- [shellcheck](https://github.com/koalaman/shellcheck): 静态 Shell 脚本分析工具, 本质是 bash/sh/zsh 的 lint
- [Filenames and Pathnames in Shell](http://www.dwheeler.com/essays/filenames-in-shell.html): 如何在脚本中正确处理文件名.
- [Data Science at the Command Line](http://datascienceatthecommandline.com/#tools)
