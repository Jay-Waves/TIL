本篇旨在**快速配置一个舒适的ubuntu环境**.

省流:
```shell
# don't put duplicate lines or lines starting with space in the history.
setopt histignorespace
setopt histignoredups

# append to the history file, don't overwrite it
setopt append_history

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'
    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'


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

### Install Ubuntu

**minimal install** ubuntu-desktop

VirtualBox Network: NAT + Host-Only

### Install Basic Software

```shell
# shell
sudo apt install zsh, tree

# build essentials
sudo apt install git, make, llvm, build-essential, gcc, cmake, sudo, 

# luanguage
sudo apt install go, rust, python

# network
sudo apt install net-tools, curl, traceroute, wireshark, wget, nfs-utils

# server
sudo apt install openssl-devel, opennssh-erver
# qt
...

# tool
sudo apt vim
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

default `.zshrc`:

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

