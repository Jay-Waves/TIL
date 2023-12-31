### 系统互通
wsl的环境变量是和windows共享的, 具体而言, wsl能直接使用windows的环境变量, 但是执行具体程序必须具名`.exe`后缀; 而windows执行wsl的环境, 需要设置环境变量WSLENV
比如wsl中和win互通剪贴板: 使用cmd的clip.exe程序; 调用剪贴板则需要powershell get-clipboard, 当然wsl-ubuntu中我把这一长串命令alias为`pst`了, 可以在./bashrc后找.
又比如python, wsl内置的用`python`, 用windows的用`python.exe`

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

vim:
- **_vimrc全局设置文件**在用户文件夹下

windows下使用linux命令的简单方法: windows平台有个GitBash虚拟终端很强大, 将它的目录下程序文件夹添加到windows的环境变量中即可. `...\Git\usr\bin`

#### 删除 wsl

```bash
wsl --list

wsl --unregister Kali-Linux
```