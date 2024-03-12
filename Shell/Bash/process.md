## Management

### `pkill`, `pgrep`

`pkill -f` 根据名称查找进程或发送信号

### `pstree`

`pstree -p`

### `bg`

Lists stopped or background jobs; resume a stopped job in the background.

### `fg`

Brings the most recent job in the foreground.

### `top`

Displays your currently active processes. `htop` in advance

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

list open files.

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

## Check

`top`, `htop`