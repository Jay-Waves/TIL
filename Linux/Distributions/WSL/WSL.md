

wsl比较坑的是主文件都在C盘, 挪不好挪.

### wslconfig

~/.wslconfig文件:

```
[wsl2]
swap=0
memory=8GB
localhostForwaring=true
```

### WSL 配置网络代理

开启 Clash 的 LAN 模式.
1. 获取局域网地址: `cat /etc/resolv.conf|grep nameserver|awk '{print $2}'`
2. `export $ALL_PROXY="http://host_ip:7890"`, 7890是clash的默认端口
3. 配置 git: `git config --global http.proxy http://host_ip:port`

注意: 似乎 TUN Mode 也能解决这个问题

git 配置:
```
git config --global http.proxy "socks5://127.0.0.1:1080"
git config --global https.proxy "socks5://127.0.0.1:1080"

查看配置:
git config --list
```

使用该git配置, 指定使用IPv4 (而不是IPv6), 可以解决 git 的 `TLS Connection Failed` 问题. 见 [StackOverflow](https://stackoverflow.com/questions/51635536/the-tls-connection-was-non-properly-terminated). 这里的`--global`选项, 可能影响到Windows主机.

## 兼容性



### 环境变量

windows 和 wsl 的环境变量是共享的. 意味着, wsl 环境可以直接使用 windows 上的程序, 但需要加 `.exe` 后缀. 如 `code.exe`.

windows 执行 wsl 环境, 需要设置 `PATH:WSLENV` 

windows 磁盘都挂在在 `/mnt` 下.

### 剪贴板

windows 剪贴板管理工具是 clip.exe, wsl 中某些情况下可能无法通过 UI 直接复制内容到 windows 剪贴板, 可以直接访问 clip.exe.

如, vim 中使用 `:'<'>w !iconv -f utf-8 -t utf-16LE | clip.exe`, 需注意 windows 平台使用的编码是 `utf-16le`.

```

### 删除 wsl

```bash
wsl --list

wsl --unregister Kali-Linux
```