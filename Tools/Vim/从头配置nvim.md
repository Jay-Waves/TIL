neovim 和 vim 兼容性好, 性能和功能多. 本篇旨在快速在 ubuntu 下创建一个可用的 nvim 环境.

```bash
mkdir -p ~/.config/nvim
nvim ~/.config/nvim/init.vim
```

`init.vim`, 可以参考[我的gist](https://gist.github.com/jay-waves/5147af04168654f0cf06fa8baf05a984)

```vim
" 设置行号显示
set number

" 启用语法高亮
syntax enable

" 设置缩进长度
set tabstop=4
set shiftwidth=4
set expandtab

" 设置搜索高亮显示
set hlsearch

" 使搜索不区分大小写
set ignorecase
set smartcase

" 启用鼠标支持
set mouse=a

" 启用真彩色支持
set termguicolors

" 设置剪贴板共享
set clipboard=unnamedplus

" 设置文件编码
set encoding=utf-8
set fileencoding=utf-8

" 启用自动折行时, 应替换对应上下行移动命令
nmap j gj
nmap k gk
```

## 安装插件管理器

```sh
sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
```

编辑 `init.vim`, 如:
```sh
call plug#begin('~/.local/share/nvim/plugged')
Plug 'preservim/nerdtree'
call plug#end()

map <C-n> :NERDTreeToggle<CR>
```

然后进入 nvim, 执行 `:PlugInstall`

## 主题

几个好看的开源主题:
- https://github.com/catppuccin/nvim
- https://github.com/projekt0n/github-nvim-theme
- https://github.com/rebelot/kanagawa.nvim
- https://github.com/rose-pine/neovim

```vim
" init.vim theme
syntax on
set termguicolors
colorscheme github_dark
let g:airline_theme="github_dark"
```

## 安装最新版 nvim 0.8.x

获取 neovim 官方 PPA:
```shell
sudo add-apt-repository ppa:neovim-ppa/unstable
sudo apt update
sudo apt install neovim
```

详见[我的gist for ubuntu](https://gist.github.com/jay-waves/21aa03ae7c05d0500c470c14706d0397)