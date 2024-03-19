### 临时上下文

使用 `(...)` 来创建临时上下文:

```bash
	# do something in current dir
	(cd /some/other/dir && other-command)
	# continue in original dir
```

### 过程替换

```bash
diff /etc/hosts <(ssh somehost cat /etc/hosts)
```

过程替换: `<(...)` 会将扩招中命令执行后, 输出到一个临时命名管道 (named pipe), 然后继续传递. 对于接收这个输入的命令而言, **就像是在读取普通文件**

### 大括号包裹代码

编写脚本时, 使用大括号包裹代码. 他人复制代码时, 可以避免复制不全, 而执行不完整的代码:

```bash
{
	#hello
}
```

### 大括号提供文本扩展

`mv foo.{txt,pdf} ../` 同时移动两个文件

`cp somefile{,.bak}` <=> `cp somfile somfile.bak`

`mkdir -p test-{a,b,c}/subtest-{1,2,3}`

### 调试脚本

在脚本开头写上调试命令:

```bash
	set -euo pipefail
	trap "echo 'error: Script failed: see failed command above'" ERR
```

可以检测错误 (如管道错误, 变量未赋值), 并在错误发生时中断程序, 捕获 ERR/EXIT, 然后输出错误信息.