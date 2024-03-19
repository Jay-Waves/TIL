`~/AppData/Local/nvim/init.vim`

```vim
set number                                      " 行号
set tabstop=4                                   " tab 宽度
set shiftwidth=4                                " 缩进宽度
set expandtab                                   " 将tab解释为空格
set cursorline                                  " 高亮当前行
syntax on
set smartindent  
set showcmd
set wrap                                        " 自动换行
set ignorecase                                  " 搜索时忽略大小写
set smartcase                                   " 如果搜索包含大写字符，则自动启用大小写敏感

" font, not for terminal
set guifont=Source\ Code\ Pro:h13

" plugins
call plug#begin()
Plug 'nightsense/snow'
Plug 'arcticicestudio/nord-vim'
Plug 'hzchirs/vim-material'
"Plug 'vim-syntastic/syntastic'                 " 语法检查插件
Plug 'vim-airline/vim-airline'                  " taskbar美化插件
Plug 'vim-airline/vim-airline-themes'
Plug 'ctrlpvim/ctrlp.vim'                       " 模糊查找文件
"Plug 'davidhalter/jedi-vim'                    " python 自动补全
call plug#end()

" keyboard
nmap j gj
nmap k gk

" theme
" snow theme
"colorscheme snow
"let g:airline_theme='snow_light'

" material theme
"colorscheme vim-material
"let g:material_style='dark'
"let g:airline_theme='material'

" nord theme
colorscheme nord
let g:airline_theme='nord'

set background=dark
set termguicolors

" taskbar
set t_Co=256                                    "色域设置
set laststatus=2                                "这个是安装字体后 必须设置此项 
let g:airline_powerline_fonts = 1   
let g:airline#extensions#tabline#enabled = 1
let g:airline_section_c = ''                    " 禁用文件名
"let g:airline_left_sep = '|'
"let g:airline_right_sep = '|'
```