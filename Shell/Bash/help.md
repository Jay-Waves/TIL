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
