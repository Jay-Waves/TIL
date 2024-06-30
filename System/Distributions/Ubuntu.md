本篇旨在**快速配置一个舒适的ubuntu环境**.
- 虚拟机代理软件: VMWare 或 VirtualBox
- 主机: Windows10
- 虚拟机: Ubuntu

### Install Ubuntu

**minimal install** ubuntu-desktop

VirtualBox Network: NAT + Host-Only

### Install Basic Software

```shell
# shell
sudo apt install zsh tree

# build essentials
sudo apt install git make llvm build-essential gcc cmake sudo 

# luanguage
sudo apt install go, rust, python

# network
sudo apt install net-tools, curl, traceroute, wireshark, wget, nfs-utils

# server
sudo apt install openssl-devel, opennssh-server
# qt
...

# tool
sudo apt install vim
```

In Ubuntu Software Store:
- install vscode
- install alacritty
- install marktext

### Terminal

Zsh, change shell:

```shell
chsh -s /bin/zsh
```

修改 `.zshrc`, 详见 [gist: .bashrc](https://gist.github.com/jay-waves/f20f48813bf9cd257e09e24e3a71d354)

```shell
# ------------- command and prompt --------------------------
# color support
# export COLORTERM=truecolor
# export TERM="xterm-256color

# prompt customize for bash: bash-it or starship
eval "$(starship init zsh)"

# command syntax highlight
source /usr/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh

# ------------- network --------------------------

# global proxy
host_ip=192.168.56.1
export all_proxy="http://$host_ip:7890"
export https_proxy="http://$host_ip:7890"
export http_proxy="http://$host_ip:7890"
export ALL_PROXY="http://$host_ip:7890"
export HTTPS_PROXY="http://$host_ip:7890"
export HTTP_PROXY="http://$host_ip:7890"

# software proxy
git config --global http.proxy "socks5://$host_ip:7890"
git config --global http.proxy "socks5://$host_ip:7890"

# ------------- compiler and lsp --------------------------

# language server
#. "$HOME/.cargo/env"

# compiler
export MAKEFLAGS="-j4"
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

# ---------- user software settings -----------------------

# add ./local to path
export PATH="$HOME/.local/bin:$PATH"

# keyboard map
xmodmap -e "keycode 9 = Caps_Lock"
xmodmap -e "keycode 66 = Escape"

# helix
export HELIX_RUNTIME=/home/dirge/bin/helix-2305/runtime

# git config
export GIT_TOKEN="..."
git config --global credential.helper 'cache --timeout=3600'
git config --global user.name "Jay-Waves"
git config --global user.email "yujiawei@buaa.edu.cn"
```

StarShip:
```shell
curl -sS https://starship.rs/install.sh | sh
```
设置自启动: 在 .bashrc 中:
```shell
eval "$(starship init zsh)"
```

Alacritty
```shell

```

### Network

proxy: ubuntu setting (?? 系统一般使用主机vpn?? 但是工具, git不使用? 这是为什么)

go proxy:

git setting:

如果不是虚拟机: 安装 Clash.AppImage, 启用vpn.

### Coding

VScode:

keyboard map: 放入 .bashrc
```bash
# 将 CapsLock 和 Escape 互换
xmodmap -e "keycode 9 = Caps_Lock"
xmodmap -e "keycode 66 = Escape"
```

### User Dir:

```
Src
Code
Local
Desktop
Blob (media)
Share (--> /mnt/share VB)
```

