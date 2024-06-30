适用于:
- 主机 Windows10
- 虚拟机代理软件 WSL (Hyper-V)
- 虚拟机 Debian
- 网络代理软件 Clash

## 配置网络

将 WSL 网络模式设置为镜像模式, 此时虚拟机同步主机的 LocalHost, 也就同步了 Clash 代理. 在 `~/.bashrc` 中添加:

```bash
# global proxy
host_ip=127.0.0.1
export all_proxy="http://$host_ip:7890"
export https_proxy="http://$host_ip:7890"
export http_proxy="http://$host_ip:7890"
export ALL_PROXY="http://$host_ip:7890"
export HTTPS_PROXY="http://$host_ip:7890"
export HTTP_PROXY="http://$host_ip:7890"

# software proxy
git config --global http.proxy "socks5://$host_ip:7890"
git config --global http.proxy "socks5://$host_ip:7890"
```

## 更新软件包

```bash
sudo apt update
```

若成功更新, 说明上一步网络配置也成功了. 下一步安装一些必要软件

```bash
# 我习惯的 shell 和 editor
sudo apt install zsh vim nvim

# 编译用
sudo apt install git make build-essential llvm

# 网络
sudo apt install net-tools curl traceroute wget
```

## 软件配置

### 配置 Zsh

修改默认 shell 为 zsh

```bash
chsh -s /bin/zsh
zsh # 进入 zsh 环境
```

安装 Prompts 增强软件 [StarShip](https://starship.rs)

```sh
curl -sS https://starship.rs/install.sh | sh
# 进入 .config 修改 starship 的配置
mkdir -p ~/.config && touch ~/.config/starship.toml
cat <<EOF > ~/.config/starship.toml
[custom.my_custom_char]
command="echo '@debian'"
when="true"
EOF
```

修改 zsh 配置 `~/.zshrc`, 详见 [gist: .zshrc](https://gist.github.com/jay-waves/a7aef5ac34215e41f6c24dc7e883e7c1)

安装 oh-my-zsh

```sh
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# 安装 oh-my-zsh 的插件
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

```

### 安装 zoxide

```sh
sudo apt install zoxide -y
```

**上述各个插件以及软件, 皆需要在 `.zshrc` 中启用, 详见 [gist: .zshrc](https://gist.github.com/jay-waves/a7aef5ac34215e41f6c24dc7e883e7c1)**

### 安装 nvim

nvim 配置文件及配置方法详见 [gist: init.vim](https://gist.github.com/jay-waves/21aa03ae7c05d0500c470c14706d0397). 

```sh
# 添加官方 PPA
sudo apt install software-properties-common -y
sudo add-apt-repository ppa:neovim-ppa/stable
sudo apt update

sudo apt install neovim -y
mkdir -p ~/.config/nvim && touch ~/.config/nvim/init.vim
```

由于 Debian PPA 库中的 Neovim 版本较旧, 部分插件可能出现错误, 此时可手动安装:

```sh
curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim-linux64.tar.gz
sudo rm -rf /opt/nvim
sudo tar -C /opt -xzf nvim-linux64.tar.gz

# 设置环境变量
export PATH="$PATH:/opt/nvim-linux64/bin"
```
