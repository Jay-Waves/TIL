### SheBang

Linux 脚本文件第一行 `shebang` 用于指定程序的执行器:

```bash
#!/usr/bin/env bash
```

```bash
#!/usr/bin/python
```

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

### 多线程

You can easily multi-threading your jobs using `&`. All those jobs will then run in the background simultaneously and you can see the processes below are running using `jobs`.

```bash
sleep 15 & sleep 5 &
```

The optional `wait` command will then wait for all the jobs to finish.

```bash
sleep 10 & sleep 5 &
wait
```