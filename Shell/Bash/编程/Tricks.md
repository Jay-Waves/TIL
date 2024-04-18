### SheBang

Linux 脚本文件第一行 `shebang` 用于指定程序的执行器:

```bash
#!/usr/bin/env bash
```

```bash
#!/usr/bin/python
```

### 大括号包裹代码

编写脚本时, 使用大括号包裹代码. 他人复制代码时, 可以避免复制不全, 而执行不完整的代码:

```bash
{
	#hello
}
```

大括号包括的代码不会影响变量作用域, 即不会像管道, `()` 一样创建临时上下文.

### 大括号提供文本扩展

`mv foo.{txt,pdf} ../` 同时移动两个文件

`cp somefile{,.bak}` <=> `cp somfile somfile.bak`

`mkdir -p test-{a,b,c}/subtest-{1,2,3}`

### 跳脱符 \

`\`可以跳脱下一个输入的字符, 也就是下一个字符输入后不再表达原本意义. 

常用于`\[Enter]`,让[Enter]不再是执行当前行命令, 而是单纯**换行**

### 查询文件

`find` 找到含有字符串'score'的文件名, 并列出其路径: `find /path/to/dir -type f | grep score`

使用find命令会自动查询文件夹*及其子文件夹*的内容, 比如`find /dir -name file_name`

找到含有字符串'score'的文件名, 并列出其路径: `find /path/to/dir -type f | grep score`

### 命令别名

Run `nano ~/.bash_profile` and add the following line:

```bash
alias dockerlogin='ssh www-data@adnan.local -p2222'  # add your alias in .bash_profile
```

`$ alias new_expr=old_instru_expr`

```bash
e.g.
$ alias lm='ls -al | more'
```

`alias`: 列出当前所有 alias

`unalias old_instru_alias`: 取消某 alias

### 命令历史

`history [n]` 显示 (过去 n 个) 编号历史

`history -c` 清除历史

`echo ${HISTSIZE}$`

`!!` 上一条命令

`!n` 执行第 n 条历史命令 (用 `^R` 更方便)

`!$$` 上一条参数

### 快速定位

Run `nano ~/.bashrc` and add the following line:

```bash
export hotellogs="/workspace/hotel-api/storage/logs"
```

Now you can use the saved path:

```bash
source ~/.bashrc
cd $hotellogs
```
