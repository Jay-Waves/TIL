bash 管理多进程非常方便.

### 多进程

使用 `&` 在后台创建多进程, 使用 `jobs` 查看后台进程.

```bash
sleep 15 & sleep 5 &
pid_slp=$! # 获取 sleep 5 的 pid
```

使用 `wait` 等待所有工作结束.

```bash
sleep 10 & sleep 5 &
wait
```

### 管理信号

捕获退出信号, 注册回调函数 finish.

```bash
function finish {
  # your cleanup here. e.g. kill any forked processes
  jobs -p | xargs kill
}
trap finish EXIT
```

捕获 Ctrl-C 信号, 释放资源.

```bash
function cleanup {
	kill pid1 pid2 pid3 ...
	wait pid1 pid2 ...
}

trap cleanup SIGINT
```

### 临时上下文

使用 `(...)` 来创建临时上下文:

```bash
	# do something in current dir
	(cd /some/other/dir && other-command)
	# continue in original dir
```