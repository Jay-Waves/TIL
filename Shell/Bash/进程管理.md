## Management

### `pkill`, `pgrep`

`pkill -f` 根据名称查找进程或发送信号

### `pstree`

`pstree -p`

### `bg`

Lists stopped or background jobs; resume a stopped job in the background.

### `fg`

Brings the most recent job in the foreground.

### `jobs`

Lists the jobs running in the background, giving the job number.

### `ps`

Lists your processes.  

```bash
ps -u yourusername

pstree -p
```

Use the flags ef. e for every process and f for full listing. 

```bash
ps -ef
```

###  `kill`

Kills (ends) the processes with the ID you gave.  

```bash
kill PID
```

### `killall`

Kill all processes with the name.  

```bash
killall processname
```

### `lsof`

list open files. 因为 Unix 系统 "万物(硬件, 套接字, 管道)皆文件" 的思想, 该命令常用于调试系统问题.

```bash
# 列出某个用户打开的文件
lsof -u username
# 列出某个端口的进程
lsof -i :port 
# 列出某个进程打开的文件
lsof -p  pid
```

### &

The `&` symbol instructs the command to run as a background process in a subshell.

```bash
command &
```

### `nohup`

nohup stands for "No Hang Up". This allows to run command/process or shell script that can continue running in the background after you log out from a shell.

```bash
nohup command
```

Combine it with `&` to create background processes 

```bash
nohup command &
```

### `time`, `timeout`

- `time`：执行命令，并计算执行时间

- `timeout`：在指定时长范围内执行命令，并在规定时间结束后停止进程

## `top`

任务管理器, 更现代的选择是 `htop`. 其几个参数意义为:
- `PRI`: Priority
- `NI`: nice value, hight priority if value is negative
- `VIRT`: 虚拟内存使用量
- `RES`: 实际使用物理内存大小
- `SHR`: 共享内存大小
- `S`: 状态, 包括: `R` running, `S` sleeping, `D` disk sleep, `T` stopped, `Z` zombie.

## `watch`, `inotifywait`

`watch` 用于周期性地执行给定命令.

```bash
# 监控新文件, 每10s
watch -n 10 'ls -lrt /path/to/monitor | tail'
# 使用 find 查找近10分钟产生的文件
watch 'find /path/to/monitor -type f -mmin -1'
```

`inotifywait` 使用 linux 的 inotify 系统, 立即响应目录变化, 无需周期性轮询检查.

```bash
# 监控新文件创建
inotifywait -m -e create /path/to/monior
```