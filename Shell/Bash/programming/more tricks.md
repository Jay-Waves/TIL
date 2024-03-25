### 跳脱符 \

`\`可以跳脱下一个输入的字符, 也就是下一个字符输入后不再表达原本意义. 

常用于`\[Enter]`,让[Enter]不再是执行当前行命令, 而是单纯**换行**

### 查询文件

`find` 找到含有字符串'score'的文件名, 并列出其路径: `find /path/to/dir -type f | grep score`

使用find命令会自动查询文件夹*及其子文件夹*的内容, 比如`find /dir -name file_name`

找到含有字符串'score'的文件名, 并列出其路径: `find /path/to/dir -type f | grep score`

### 别名

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

### Exit traps

Make your bash scripts more robust by reliably performing cleanup.

```bash
function finish {
  # your cleanup here. e.g. kill any forked processes
  jobs -p | xargs kill
}
trap finish EXIT
```

### Accessing your scripts

You can easily access your scripts by creating a bin folder in your home with `mkdir ~/bin`, now all the scripts you put in this folder you can access in any directory.

If you can not access, try append the code below in your `~/.bash_profile` file and after do `source ~/.bash_profile`.

```bash
# set PATH so it includes user's private bin if it exists
if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi
```## Exit traps

Make your bash scripts more robust by reliably performing cleanup.

```bash
function finish {
  # your cleanup here. e.g. kill any forked processes
  jobs -p | xargs kill
}
trap finish EXIT
```
