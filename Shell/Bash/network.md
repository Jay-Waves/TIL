## Network Info

windows 上原生网络工具请见 

### `ip`

`ip addr show`

### `mtr`

[`mtr`](http://www.bitwizard.nl/mtr/), my traceroute, = traceroute+ping . mtr 用于跟踪路由, 提供了交互式界面, 可以灵活调整参数.

### `netcat`, `socat`

`netcat` (or `nc`) 用于读取和写入网络连接. 常见用途为端口扫描, 监听传入连接, 端口重定向和测试. 当然, 端口扫描更专业的工具是 `nmap`, `netcat` 监听链接时有安全问题.

```sh
nc -l 1234           # 监听端口, 作为服务器
nc example.com 1234  # 连接到主机端口, 作为客户端
```

`socat` 是加强版 `netcat`:

```sh
# 转发 tcp 端口
socat TCP-LISTEN:port1,fork TCP:target.address:port2
```

### `nslookup` 

根据域名查看 ip.

### `dig`

Gets DNS information for domain.  domain info get.

```bash
dig domain
```

### `whois`

Gets whois information for domain.  

```bash
whois domain
```

## Network Debug

### `ping`

Pings host and outputs results.  

```bash
ping host
```

### `netstat`, `ss`

`netstat` (network statistics) 用来显示套接字信息.

- `t` 显示 TCP 连接
- `u` 显示 UDP 连接
- `l` 仅显示处于监听状态的服务端口
- `a` 显示所有套接字 (包括非监听状态的)
- `n` 显示地址和端口号的数字形式 (不解析为名称)
- `p` 显示每个连接对应的进程ID和进程名

```bash
# 监听端口进程
netstat -lntp
ss -plat

# 用 lsof 也可实现, 显然更麻烦.
lsof -iTCP -sTCP:LISTEN -P -n
```

`ss` (socket statistics) 比 `netstat` 更加现代, 处理速度更快.

### `wireshark`, `tshark`

[`wireshark`](https://wireshark.org/)，[`tshark`](https://www.wireshark.org/docs/wsug_html_chunked/AppToolstshark.html) 和 [`ngrep`](http://ngrep.sourceforge.net/) 可用于复杂的网络调试

## Network Connection Tool

### `scp`

Transfer files between a local host and a remote host or between two remote hosts.

*copy from local host to remote host*

```bash
scp source_file user@host:directory/target_file
```

*copy from remote host to local host*

```bash
scp user@host:directory/source_file target_file
scp -r user@host:directory/source_folder target_folder
```

This command also accepts an option `-P` that can be used to connect to specific port.  

```bash
scp -P port user@host:directory/source_file target_file
```

### `rsync`

Does the same job as `scp` command, but transfers only changed files. Useful when transferring the same folder to/from server multiple times.

```bash
rsync source_folder user@host:target_folder
rsync user@host:target_folder target_folder
```

### `ssh`

请查看: [Network/应用层/ssh](../../Network/应用层/ssh.md)

### `curl`, `wget`

查看 [Networl/Tool/curl](../../Network/Tool/curl.md), 或者试试更潮的 [httpie](https://github.com/jkbrzt/httpie)

<pre>
$ wget file_url
</pre>

