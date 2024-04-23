
- [简介](#简介)
  - [我正在使用什么样的 Vim](#我正在使用什么样的-vim)
  - [备忘录](#备忘录)
- [基础](#基础)
  - [缓冲区，窗口，标签](#缓冲区窗口标签)
  - [已激活、已载入、已列出、已命名的缓冲区](#已激活已载入已列出已命名的缓冲区)
  - [参数列表](#参数列表)
  - [按键映射](#按键映射)
  - [映射前置键](#映射前置键)
  - [寄存器](#寄存器)
  - [范围](#范围)
  - [标注](#标注)
  - [补全](#补全)
  - [动作，操作符，文本对象](#动作操作符文本对象)
  - [自动命令](#自动命令)
  - [变更历史，跳转历史](#变更历史跳转历史)
  - [内容变更历史记录](#内容变更历史记录)
  - [全局位置信息表，局部位置信息表](#全局位置信息表局部位置信息表)
  - [宏](#宏)
  - [颜色主题](#颜色主题)
  - [折叠](#折叠)
  - [会话](#会话)
  - [局部化](#局部化)
- [用法](#用法)
  - [获取离线帮助](#获取离线帮助)
  - [获取离线帮助（补充）](#获取离线帮助补充)
  - [获取在线帮助](#获取在线帮助)
  - [执行自动命令](#执行自动命令)
    - [用户自定义事件](#用户自定义事件)
    - [事件嵌套](#事件嵌套)
  - [剪切板](#剪切板)
    - [剪贴板的使用（Windows, OSX）](#剪贴板的使用windows-osx)
    - [剪贴板的使用（Linux, BSD, ...）](#剪贴板的使用linux-bsd-)
  - [打开文件时恢复光标位置](#打开文件时恢复光标位置)
  - [临时文件](#临时文件)
    - [备份文件](#备份文件)
    - [交换文件](#交换文件)
    - [撤销文件](#撤销文件)
    - [viminfo 文件](#viminfo-文件)
    - [临时文件管理设置示例](#临时文件管理设置示例)
  - [编辑远程文件](#编辑远程文件)
  - [插件管理](#插件管理)
  - [多行编辑](#多行编辑)
  - [使用外部程序和过滤器](#使用外部程序和过滤器)
  - [Cscope](#cscope)
    - [1. 构建数据库](#1-构建数据库)
    - [2. 添加数据库](#2-添加数据库)
    - [3. 查询数据库](#3-查询数据库)
  - [MatchIt](#matchit)
    - [在 Vim 8 中安装](#在-vim-8-中安装)
    - [在 Vim 7 或者更早的版本中安装](#在-vim-7-或者更早的版本中安装)
    - [简短的介绍](#简短的介绍)
- [技巧](#技巧)
  - [跳至选择的区域另一端](#跳至选择的区域另一端)
  - [聪明地使用 n 和 N](#聪明地使用-n-和-n)
  - [聪明地使用命令行历史](#聪明地使用命令行历史)
  - [智能 Ctrl-l](#智能-ctrl-l)
  - [禁用错误报警声音和图标](#禁用错误报警声音和图标)
  - [快速移动当前行](#快速移动当前行)
  - [快速添加空行](#快速添加空行)
    - [运行时检测](#运行时检测)
    - [查看启动时间](#查看启动时间)
  - [NUL 符用新行表示](#nul-符用新行表示)
  - [快速编辑自定义宏](#快速编辑自定义宏)
  - [快速跳转到源(头)文件](#快速跳转到源头文件)
  - [在 GUI 中快速改变字体大小](#在-gui-中快速改变字体大小)
  - [根据模式改变光标类型](#根据模式改变光标类型)
  - [防止水平滑动的时候失去选择](#防止水平滑动的时候失去选择)
  - [选择当前行至结尾，排除换行符](#选择当前行至结尾排除换行符)
  - [重新载入保存文件](#重新载入保存文件)
  - [更加智能的当前行高亮](#更加智能的当前行高亮)
  - [更快的关键字补全](#更快的关键字补全)
  - [改变颜色主题的默认外观](#改变颜色主题的默认外观)
  - [命令](#命令)
    - [:global 和 :vglobal - 在所有匹配行执行命令](#global-和-vglobal---在所有匹配行执行命令)
    - [:normal 和 :execute - 脚本梦之队](#normal-和-execute---脚本梦之队)
    - [重定向消息](#重定向消息)
- [调试](#调试)
  - [常规建议](#常规建议)
  - [调整日志等级](#调整日志等级)
  - [查看启动日志](#查看启动日志)
  - [查看运行时日志](#查看运行时日志)
  - [Vim 脚本调试](#vim-脚本调试)
  - [语法文件调试](#语法文件调试)
- [杂项](#杂项)
  - [附加资源](#附加资源)
  - [Vim 配置集合](#vim-配置集合)
  - [常见问题](#常见问题)
    - [编辑小文件时很慢](#编辑小文件时很慢)
    - [编辑大文件的时候很慢](#编辑大文件的时候很慢)
    - [持续粘贴（为什么我每次都要设置 'paste' 模式）](#持续粘贴为什么我每次都要设置-paste-模式)
    - [在终端中按 ESC 后有延时](#在终端中按-esc-后有延时)
    - [无法重复函数中执行的搜索](#无法重复函数中执行的搜索)

<!-- vim-markdown-toc -->


# 基础


## 全局位置信息表，局部位置信息表

在某一个动作返回一系列「位置」的时候，我们可以利用「全局位置信息表」和「局部位置信息表」来存储这些位置信息，方便以后跳转回对应的位置。每一个存储的位置包括文件名、行号和列号。

比如，编译代码是出现错误，这时候我们就可以把错误的位置直接显示在全局位置信息表，或者通过外部抓取工具使位置显示在局部位置信息表中。

尽管我们也可以把这些信息显示到一个空格缓冲区中，但用这两个信息表显示的好处在于接口调用很方便，而且也便于浏览输出。

Vim 中，全局位置信息表只能有一个，但每一个窗口都可以有自己的局部位置信息表。这两个信息表的外观看上去很类似，但在操作上会稍有不同。

以下为两者的操作比较：

| 动作         | 全局位置信息表 | 局部位置信息表 |
| ------------ | -------------- | -------------- |
| 打开窗口     | `:copen`       | `:lopen`       |
| 关闭窗口     | `:cclose`      | `:lclose`      |
| 下一个条目   | `:cnext`       | `:lnext`       |
| 上一个条目   | `:cprevious`   | `:lprevious`   |
| 第一个条目   | `:cfirst`      | `:lfirst`      |
| 最后一个条目 | `:clast`       | `:llast`       |

请参阅 `:h :cc` 以及底下的内容，来获取更多命令的帮助。

**应用实例**：
如果我们想用 `grep` 递归地在当前文件夹中寻找某个关键词，然后把输出结果放到全局位置信息表中，只需要这样：

```vim
:let &grepprg = 'grep -Rn $* .'
:grep! foo
<grep output - hit enter>
:copen
```

执行了上面的代码，你就能看到所有包含字符串 "foo" 的文件名以及匹配到的相关字段都会显示在全局位置信息表中。

返回主目录 [:arrow_heading_up:](#基础)

## 会话

如果你保存了当前的「视图」（请参阅 `:h :mkview`），那么当前窗口、配置和按键映射都会被保存下来（请参阅 `:h :loadview`）。

「会话」就是存储所有窗口的相关设置，以及全局设置。简单来说，就是给当前的 Vim 运行实例拍个照，然后把相关信息存储到会话文件中。存储之后的改动就不会在会话文件中显示，你只需要在改动后更新一下会话文件就可以了。

你可以把当前工作的「项目」存储起来，然后可以在不同的「项目」之间切换。

现在就来试试吧。打开几个窗口和标签，然后执行 `:mksession Foo.vim`。如果你没有指定文件名，那就会默认保存为 `Session.vim`。这个文件会保存在当前的目录下，你可以通过 `:pwd` 来显示当前路径。重启 Vim 之后，你只需要执行 `:source Foo.vim`，就可以恢复刚才的会话了。所有的缓冲区、窗口布局、按键映射以及工作路径都会恢复到保存时的状态。

其实 Vim 的会话文件就只是 Vim 命令的集合。你可以通过命令 `:vs Foo.vim` 来看看会话文件中究竟有什么。

你可以决定 Vim 会话中究竟要保存哪些配置，只需要设置一下 `'sessionoptions'` 就可以了。

为了方便开发，Vim 把最后一次调用或写入的会话赋值给了一个内部变量 `v:this_session`。

请参阅以下文档来获取更多帮助：

```vim
:h Session
:h 'sessionoptions'
:h v:this_session
```

## 局部化

以上提到的很多概念，都有一个局部化（非全局）的版本：

| 全局        | 局部                  | 作用域       | 帮助文档              |
| ----------- | --------------------- | ------------ | --------------------- |
| `:set`      | `:setlocal`           | 缓冲区或窗口 | `:h local-options`    |
| `:map`      | `:map <buffer>`       | 缓冲区       | `:h :map-local`       |
| `:autocmd`  | `:autocmd * <buffer>` | 缓冲区       | `:h autocmd-buflocal` |
| `:cd`       | `:lcd`                | 窗口         | `:h :lcd`             |
| `:<leader>` | `:<localleader>`      | 缓冲区       | `:h maploacalleader`  |

变量也有不同的作用域，详细内容请参考 [Vim scripting 的文档](http://vimdoc.sourceforge.net/htmldoc/usr_41.html)。

# 用法

## 获取离线帮助

## 执行自动命令

你可以触发任何事件，如：`:doautocmd BufRead`。

### 用户自定义事件

对于插件而言，创建你自己的自定义事件有时非常有用。

```vim
function! Chibby()
    " A lot of stuff is happening here.
    " And at last..
    doautocmd User ChibbyExit
endfunction
```

现在你插件的用户可以在 Chibby 执行完成之后做任何他想做的事情：

```vim
autocmd User ChibbyExit call ChibbyCleanup()
```

顺便提一句，如果在使用 `:autocmd` 或 `:doautocmd` 时没有捕捉异常，那么会输出 "No matching autocommands" 信息。这也是为什么许多插件用 `silent doautocmd ...` 的原因。但是这也会有不足，那就是你不能再在 :autocmd 中使用 `echo "foo"` 了，取而代之的是你要使用 `unsilent echo "foo"` 来输出。

这就是为什么要在触发事件之前先判断事件是否存在的原因，

```vim
if exists('#User#ChibbyExit')
  doautocmd User ChibbyExit
endif
```

帮助文档：`:h User`

### 事件嵌套

默认情况下，自动命令不能嵌套！如果某个自动命令执行了一个命令，这个命令再依次触发其它的事件，这是不可能的。

例如你想在每次启动 Vim 的时候自动打开你的 vimrc 文件：

```vim
autocmd VimEnter * edit $MYVIMRC
```

当你启动 Vim 的时候，它会帮你打开你的 vimrc 文件，但是你很快会注意到这个文件没有任何的高亮，尽管平时它是正常可以高亮的。

问题在于你的非嵌套自动命令 `:edit` 不会触发“BufRead”事件，所以并不会把文件类型设置成“vim”，进而 `$VIMRUNTIME/syntax/vim.vim` 永远不会被引入。详细信息请参考：`:au BufRead *.vim`。要想完成上面所说的需求，使用下面这个命令：

```vim
autocmd VimEnter * nested edit $MYVIMRC
```

帮助文档：`:h autocmd-nested`

## 剪切板


## 打开文件时恢复光标位置

如果没有这个设置，每次打开文件时光标都将定位在第一行。而加入了这个设置以后，你就可以恢复到上次关闭文件时光标所在的位置了。

将下面的配置添加到你的 vimrc 文件：

```vim
autocmd BufReadPost *
    \ if line("'\"") > 1 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif
```

这是通过判断之前的光标位置是否存在（文件可能被其它程序修改而导致所记录的位置已经不存在了），如果存在的话就执行 `` g`" `` （转到你离开时的光标位置但是不更改跳转列表）。

这需要使用 viminfo 文件：`:h viminfo-`。

## 临时文件

根据选项的不同， Vim 最多会创建 4 种工作文件。

### 备份文件

你可以让 Vim 在将修改写入到文件之前先备份原文件。默认情况下， Vim 会保存一个备份文件但是当修改成功写入后会立即删除它（`:set writebackup`）。如果你想一直保留这个备份文件的话，可以使用 `:set backup`。而如果你想禁用备份功能的话，可以使用 `:set nobackup nowritebackup`。

咱们来看一下上次我在 vimrc 中改了什么：

```sh
$ diff ~/.vim/vimrc ~/.vim/files/backup/vimrc-vimbackup
390d389
< command! -bar -nargs=* -complete=help H helpgrep <args>
```

帮助文档：`:h backup`

### 交换文件

假设你有一个非常棒的科幻小说的构思。在按照故事情节已经写了好几个小时几十万字的时候..忽然停电了！而那时你才想起来你上次保存 `~/来自外太空的邪恶入侵者.txt` 是在.. 好吧，你从来没有保存过。

但是并非没有希望了！在编辑某个文件的时候， Vim 会创建一个交换文件，里面保存的是对当前文件所有未保存的修改。自己试一下，打开任意的文件，并使用 `:swapname` 获得当前的交换文件的保存路径。你也可以将 `:set noswapfile` 加入到 vimrc 中来禁用交换文件。

默认情况下，交换文件会自动保存在被编辑文件所在的目录下，文件名以 `.file.swp` 后缀结尾，每当你修改了超过 200 个字符或是在之前 4 秒内没有任何动作时更新它的内容，在你不再编辑这个文件的时候会被删除。你可以自己修改这些数字，详见：`:h 'updatecount'` 和 `:h 'updatetime'`。

而在断电时，交换文件并不会被删除。当你再次打开 `vim ~/来自外太空的邪恶入侵者.txt` 时， Vim 会提示你恢复这个文件。

帮助文档：`:h swap-file` 和 `:h usr_11`

### 撤销文件

[内容变更历史记录](#%E5%86%85%E5%AE%B9%E5%8F%98%E6%9B%B4%E5%8E%86%E5%8F%B2%E8%AE%B0%E5%BD%95)是保存在内存中的，并且会在 Vim 退出时清空。如果你想让它持久化到磁盘中，可以设置 `:set undofile`。这会把文件 `~/foo.c` 的撤销文件保存在 `~/foo.c.un~`。

帮助文档：`:h 'undofile'` 和 `:h undo-persistence`

### viminfo 文件

备份文件、交换文件和撤销文件都是与文本状态相关的，而 viminfo 文件是用来保存在 Vim 退出时可能会丢失的其它的信息的。包括历史记录（命令历史、搜索历史、输入历史）、寄存器内容、标注、缓冲区列表、全局变量等等。

默认情况下，viminfo 被保存在 `~/.viminfo`。

帮助文档：`:h viminfo` 和 `:h 'viminfo'`

### 临时文件管理设置示例

如果你跟我一样，也喜欢把这些文件放到一个位置（如：`~/.vim/files`）的话，可以使用下面的配置：

```vim
" 如果文件夹不存在，则新建文件夹
if !isdirectory($HOME.'/.vim/files') && exists('*mkdir')
  call mkdir($HOME.'/.vim/files')
endif

" 备份文件
set backup
set backupdir   =$HOME/.vim/files/backup/
set backupext   =-vimbackup
set backupskip  =
" 交换文件
set directory   =$HOME/.vim/files/swap//
set updatecount =100
" 撤销文件
set undofile
set undodir     =$HOME/.vim/files/undo/
" viminfo 文件
set viminfo     ='100,n$HOME/.vim/files/info/viminfo
```

注意：如果你在一个多用户系统中编辑某个文件时， Vim 提示你交换文件已经存在的话，可能是因为有其他的用户此时正在编辑这个文件。而如果将交换文件放到自己的 home 目录的话，这个功能就失效了。因此服务器非常不建议将这些文件修改到 HOME 目录，避免多人同时编辑一个文件，却没有任何警告。


## 插件管理

## 多行编辑

这是一种可以同时输入多行连续文本的技术。参考这个[示例](https://raw.githubusercontent.com/mhinz/vim-galore/master/contents/images/content-block_insert.gif)。

用 `<c-v>` 切换到可视块模式。然后向下选中几行，按 `I` 或 `A` （译者注：大写字母，即 shift+i 或 shift+a）然后开始输入你想要输入的文本。

在刚开始的时候可能会有些迷惑，因为文本只出现在了当前编辑的行，只有在当前的插入动作结束后，之前选中的其它行才会出现插入的文本。

举一个简单的例子：`<c-v>3jItext<esc>`。

如果你要编辑的行长度不同，但是你想在他们后面追加相同的内容的话，可以试一下这个：`<c-v>3j$Atext<esc>`。

有时你可能需要把光标放到当前行末尾之后，默认情况下你是不可能做到的，但是可能通过设置 `virtualedit` 选项达到目的：

```vim
set virtualedit=all
```

设置之后 `$10l` 或 `90|` 都会生效，即使超过了行尾的长度。

详见 `:h blockwise-examples`。在开始的时候可能会觉得有些复杂，但是它很快就会成为你的第二天性的。

如果你想探索更有趣的事情，可以看看[多光标](https://github.com/terryma/vim-multiple-cursors)

## 使用外部程序和过滤器

免责声明：Vim 是单线程的，因此在 Vim 中以前端进程执行其它的程序时会阻止其它的一切。当然你可以使用 Vim 程序接口，如 Lua，并且使用它的多线程支持，但是在那期间， Vim 的处理还是被阻止了。Neovim 添加了任务 API 解决了此问题。

（据说 Bram 正在考虑在 Vim 中也添加任务控制。如果你使用了较新版本的的 Vim ，可以看一下 `:helpgrep startjob`。）

使用 `:!` 启动一个新任务。如果你想列出当前工作目录下的所有文件，可以使用 `:!ls`。 用 `|` 来将结果通过管道重定向，如：`:!ls -l | sort | tail -n5`。

没有使用范围时（译者注：范围就是 `:` 和 `!` 之间的内容，`.` 表示当前行，`+4` 表示向下偏移 4 行，`$` 表示最末行等，多行时用 `,` 将它们分开，如 `.,$` 表示从当前行到末行），`:!` 会显示在一个可滚动的窗口中（译者注：在 GVim 和在终端里运行的结果稍有不同）。相反的，如果指定了范围，这些行会被[过滤](<https://en.wikipedia.org/wiki/Filter_(software)>)。这意味着它们会通过管道被重定向到过滤程序的 [stdin](https://en.wikipedia.org/wiki/Standard_streams#Standard_input_.28stdin.29)，在处理后再通过过滤程序的 [stdout](https://en.wikipedia.org/wiki/Standard_streams#Standard_output_.28stdout.29) 输出，用输出结果替换范围内的文本。例如：为接下来的 5 行文本添加行号，可以使用：

```vim
:.,+4!nl -ba -w1 -s' '
```

由于手动添加范围很麻烦， Vim 提供了一些辅助方法以方便的添加范围。如果需要经常带着范围的话，你可以在可见模式中先选择，然后再按 `:` （译者注：选中后再按 `!` 更方便）。还可以使用 `!` 来取用一个 motion 的范围，如 `!ipsort` （译者注：原文为 `!ip!sort` ，但经过实验发现该命令执行报错，可能是因为 Vim 版本的原因造成的，新版本使用 `ip` 选择当前段落后自动在命令后添加了 `!` ，按照作者的写法来看，可能之前的版本没有自动添加 `!` ）可以将当前段落的所有行按字母表顺序进行排序。

一个使用过滤器比较好的案例是[Go 语言](https://golang.org/)。它的缩进语法非常个性，甚至还专门提供了一个名为 `gofmt` 的过滤器来对 Go 语言的源文件进行正确的缩进。Go 语言的插件通常会提供一个名为 `:Fmt` 的函数，这个函数就是执行了 `:%!gofmt` 来对整个文件进行缩进。

人们常用 `:r !prog` 将 prog 程序的插入放到当前行的下面，这对于脚本来说是很不错的选择，但是在使用的过程中我发现 `!!ls` 更加方便，它会用输出结果替换当前行的内容。（译者注：前面命令中的 `prog` 只是个占位符，在实际使用中需要替换成其它的程序，如 `:r !ls`，这就与后面的 `!!ls` 相对应了，两者唯一的不同是第一个命令不会覆盖当前行内容，但是第二个命令会）

帮助文档：

```vim
:h filter
:h :read!
```


# 技巧


## 聪明地使用命令行历史

我（原作者）习惯用 <kbd>Ctrl</kbd> + <kbd>p</kbd> 和 <kbd>Ctrl</kbd> + <kbd>n</kbd> 来跳转到上一个/下一个条目。其实这个操作也可以用在命令行中，快速调出之前执行过的命令。

不仅如此，你会发现 <kbd>上</kbd> 和 <kbd>下</kbd> 其实更智能。如果命令行中已经存在了一些文字，我们可以通过按方向键来匹配已经存在的内容。比如，命令行中现在是 `:echo`，这时候我们按 <kbd>上</kbd>，就会帮我们补全成 `:echo "Vim rocks!"`（前提是，之前输入过这段命令）。

当然，Vim 用户都不愿意去按方向键，事实上我们也不需要去按，只需要设置这样的映射：

```vim
cnoremap <c-n> <down>
cnoremap <c-p> <up>
```

这个功能，我（原作者）每天都要用很多次。

## 智能 Ctrl-l

<kbd>Ctrl</kbd> + <kbd>l</kbd> 的默认功能是清空并「重新绘制」当前的屏幕，就和 `:redraw!` 的功能一样。下面的这个映射就是执行重新绘制，并且取消通过 `/` 和 `?` 匹配字符的高亮，而且还可以修复代码高亮问题（有时候，由于多个代码高亮的脚本重叠，或者规则过于复杂，Vim 的代码高亮显示会出现问题）。不仅如此，还可以刷新「比较模式」（请参阅 `:help diff-mode`）的代码高亮：

```vim
nnoremap <leader>l :nohlsearch<cr>:diffupdate<cr>:syntax sync fromstart<cr><c-l>
```

### 运行时检测

需要的特性：+profile

Vim 提供了一个内置的运行时检查功能，能够找出运行慢的代码。

`:profile` 命令后面跟着子命令来确定要查看什么。

如果你想查看所有的：

```Vim
:profile start /tmp/profile.log
:profile file *
:profile func *
<do something in Vim>
<quit Vim>
```

Vim 不断地在内存中检查信息，只在退出的时候输出出来。（Neovim 已经解决了这个问题用 `:profile dump` 命令）

看一下 `/tmp/profile.log` 文件，检查时运行的所有代码都会被显示出来，包括每一行代码运行的频率和时间。

大多数代码都是用户不熟悉的插件代码，如果你是在解决一个确切的问题，
直接跳到这个日志文件的末尾，那里有 `FUNCTIONS SORTED ON TOTAL TIME` 和 `FUNCTIONS SORTED ON SELF TIME` 两个部分，如果某个 function 运行时间过长一眼就可以看到。

### 查看启动时间

感觉 Vim 启动的慢？到了研究几个数字的时候了：

```vim
vim --startuptime /tmp/startup.log +q && vim /tmp/startup.log
```

第一栏是最重要的因为它显示了**绝对运行时间**，如果在前后两行之间时间差有很大的跳跃，那么是第二个文件太大或者含有需要检查的错误的 VimL 代码。

## NUL 符用新行表示

文件中的 NUL 符 （`\0`），在内存中被以新行（`\n`）保存，在缓存空间中显示为 `^@`。

更多信息请参看 `man 7 ascii` 和 `:h NL-used-for-Nul` 。

## 快速编辑自定义宏

这个功能真的很实用！下面的映射，就是在一个新的命令行窗口中读取某一个寄存器（默认为 `*`）。当你设置完成后，只需要按下 <kbd>回车</kbd> 即可让它生效。

在录制宏的时候，我经常用这个来更改拼写错误。

```vim
nnoremap <leader>m  :<c-u><c-r><c-r>='let @'. v:register .' = '. string(getreg(v:register))<cr><c-f><left>
```

只需要连续按下 <kbd>leader</kbd> <kbd>m</kbd> 或者 <kbd>"</kbd> <kbd>leader</kbd> <kbd>m</kbd> 就可以调用了。

请注意，这里之所以要写成 `<c-r><c-r>` 是为了确保 `<c-r>` 执行了。请参阅 `:h c_^R^R`

## 快速跳转到源(头)文件

这个技巧可以用在多种文件类型中。当你从源文件或者头文件中切换到其他文件的时候，这个技巧可以设置「文件标记」（请参阅 `:h marks`），然后你就可以通过连续按下 <kbd>'</kbd> <kbd>C</kbd> 或者 <kbd>'</kbd> <kbd>H</kbd> 快速跳转回去（请参阅 `:h 'A`）。

```vim
autocmd BufLeave *.{c,cpp} mark C
autocmd BufLeave *.h       mark H
```

**注意**：由于这个标记是设置在 viminfo 文件中，因此请先确认 `:set viminfo?` 中包含了 `:h viminfo-'`。


## 防止水平滑动的时候失去选择

如果你选中了一行或多行，那么你可以用 <kbd>&lt;</kbd> 或 <kbd>></kbd> 来调整他们的缩进。但在调整之后就不会保持选中状态了。

你可以连续按下 <kbd>g</kbd> <kbd>v</kbd> 来重新选中他们，请参考 `:h gv`。因此，你可以这样来配置映射：

```vim
xnoremap <  <gv
xnoremap >  >gv
```

设置好之后，在可视模式中使用 `>>>>>` 就不会再出现上面提到的问题了。

## 更快的关键字补全

关键字补全（`<c-n>` 或 `<c-p>`）功能的工作方式是，无论 `'complete'` 设置中有什么，它都会尝试着去补全。这样，一些我们用不到的标签也会出现在补全列表中。而且，它会扫描很多文件，有时候运行起来非常慢。如果你不需要这些，那么完全可以像这样把它们禁用掉：

```vim
set complete-=i   " disable scanning included files
set complete-=t   " disable searching tags
```

## 改变颜色主题的默认外观

如果你想让状态栏在颜色主题更改后依然保持灰色，那么只需要这样设置：

```vim
autocmd ColorScheme * highlight StatusLine ctermbg=darkgray cterm=NONE guibg=darkgray gui=NONE
```

同理，如果你想让某一个颜色主题（比如 "lucius"）的状态栏为灰色（请使用 `:echo color_name` 来查看当前可用的所有颜色主题）：

```vim
autocmd ColorScheme lucius highlight StatusLine ctermbg=darkgray cterm=NONE guibg=darkgray gui=NONE
```

## 命令

下面的命令都比较有用，最好了解一下。用 `:h :<command name>` 来了解更多关于它们的信息，如：`:h :global`。

### :global 和 :vglobal - 在所有匹配行执行命令

在所有符合条件的行上执行某个命令。如： `:global /regexp/ print` 会在所有包含 "regexp" 的行上执行 `print` 命令（译者注：regexp 有正则表达式的意思，该命令同样支持正则表达式，在所有符合正则表达式的行上执行指定的命令）。

趣闻：你们可能都知道老牌的 grep 命令，一个由 Ken Thompson 编写的过滤程序。它是干什么用的呢？它会输出所有匹配指定正则表达式的行！现在猜一下 `:global /regexp/ print` 的简写形式是什么？没错！就是 `:g/re/p` 。 Ken Thompsom 在编写 grep 程序的时候是受了 vi `:global` 的启发。（译者注： <https://robots.thoughtbot.com/how-grep-got-its-name）>

既然它的名字是 `:global`，理应仅作用在所有行上，但是它也是可以带范围限制的。假设你想使用 `:delete` 命令删除从当前行到下一个空行（由正则表达式 `^$` 匹配）范围内所有包含 "foo" 的行：

```vim
:,/^$/g/foo/d
```

如果要在所有 _不_ 匹配的行上执行命令的话，可以使用 `:global!` 或是它的别名 `:vglobal` （ V 代表的是 inVerse ）。

### :normal 和 :execute - 脚本梦之队

这两个命令经常在 Vim 的脚本里使用。

借助于 `:normal` 可以在命令行里进行普通模式的映射。如：`:normal! 4j` 会令光标下移 4 行（由于加了"!"，所以不会使用自定义的映射 "j"）。

需要注意的是 `:normal` 同样可以使用范围数（译者注：参考 `:h range` 和 `:h :normal-range` 了解更多），故 `:%norm! Iabc` 会在所有行前加上 "abc"。

借助于 `:execute` 可以将命令和表达式混合在一起使用。假设你正在编辑一个 C 语言的文件，想切换到它的头文件：

```vim
:execute 'edit' fnamemodify(expand('%'), ':r') . '.h'
```

（译者注：头文件为与与源文件同名但是扩展名为 `.h` 的文件。上面的命令中 expand 获得当前文件的名称，fnamemodify 获取不带扩展名的文件名，再连上 '.h' 就是头文件的文件名了，最后在使用 edit 命令打开这个头文件。）

这两个命令经常一起使用。假设你想让光标下移 n 行：

```vim
:let n = 4
:execute 'normal!' n . 'j'
```

### 重定向消息

许多命令都会输出消息，`:redir` 用来重定向这些消息。它可以将消息输出到文件、[寄存器](#寄存器)或是某个变量中。

```vim
" 将消息重定向到变量 `neatvar` 中
:redir => neatvar
" 打印所有寄存器的内容
:reg
" 结束重定向
:redir END
" 输出变量
:echo neatvar
" 恶搞一下，我们把它输出到当前缓冲区
:put =neatvar
```

再 Vim 8 中，可以更简单的方式即位：

    :put =execute('reg')

（译者注：原文最后一条命令是 `:put =nicevar` 但是实际会报变量未定义的错误）
（实测 neovim/vim8 下没问题）

帮助文档：`:h :redir`

# 调试

## 常规建议

如果你遇到了奇怪的行为，尝试用这个命令重现它：

    vim -u NONE -N

这样会在不引用 vimrc（默认设置）的情况下重启 vim，并且在 **nocompatible** 模式下（使用 vim 默认设置而不是 vi 的）。（搜索 `:h --noplugin` 命令了解更多启动加载方式）

如果仍旧能够出现该错误，那么这极有可能是 vim 本身的 bug，请给 [vim_dev]("https://groups.google.com/forum/#!forum/vim_dev") 发送邮件反馈错误，多数情况下问题不会立刻解决，你还需要进一步研究

许多插件经常会提供新的（默认的/自动的）操作。如果在保存的时候发生了，那么请用 `:verb au BufWritePost` 命令检查潜在的问题

如果你在使用一个插件管理工具，将插件行注释调，再进行调试。

问题还没有解决？如果不是插件的问题，那么肯定是你的自定义的设置的问题，可能是你的 options 或 autocmd 等等。

到了一行行代码检查的时候了，不断地排除缩小检查范围知道你找出错误，根据二分法的原理你不会花费太多时间的。

在实践过程中，可能就是这样，把 `:finish` 放在你的 **vimrc** 文件中间，Vim 会跳过它之后的设置。如果问题还在，那么问题就出在`:finish`之前的设置中，再把`:finish`放到前一部分设置的中间位置。否则问题就出现在它后面的半部分设置，那么就把`:finish`放到后半部分的中间位置。不断的重复即可找到。

## 调整日志等级

Vim 现在正在使用的另一个比较有用的方法是增加 debug 信息输出详细等级。现在 Vim 支持 9 个等级，可以用`:h 'verbose'`命令查看。

```vim
:e /tmp/foo
:set verbose=2
:w
:set verbose=0
```

这可以显示出所有引用的文件、没有变化的文件或者各种各样的作用于保存的插件。

如果你只是想用简单的命令来提高等级，也是用 `:verbose` ，放在其他命令之前，通过计数来指明等级，默认是 1.

```vim
:verb set verbose
"  verbose=1
:10verb set verbose
"  verbose=10
```

通常用等级 1 来显示上次从哪里设置的选项

```vim
:verb set ai?
"      Last set from ~/.vim/vimrc
```

一般等级越高输出信息月详细。但是不要害怕，亦可以把输出导入到文件中：

```vim
:set verbosefile=/tmp/foo | 15verbose echo "foo" | vsplit /tmp/foo
```

你可以一开始的时候就打开 verbosity，用 `-V` 选项，它默认设置调试等级为 10。 例如：`vim -V5`

## 查看启动日志

## 查看运行时日志

## Vim 脚本调试

如果你以前使用过命令行调试器的话，对于`:debug`命令你很快就会感到熟悉。

只需要在任何其他命令之前加上`:debug`就会让你进入调试模式。也就是，被调试的 Vim 脚本会在第一行停止运行，同时该行会被显示出来。

想了解可用的 6 个调试命令，可以查阅`:h >cont`和阅读下面内容。需要指出的是，类似 gdb 和其他相似调试器，调试命令可以使用它们的简短形式：`c`、 `q`、`n`、`s`、 `i`和 `f`。

除了上面的之外，你还可以自由地使用任何 Vim 的命令。比如，`:echo myvar`，该命令会在当前的脚本代码位置和上下文上被执行。

只需要简单使用`:debug 1`，你就获得了[REPL](https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop)调试特性。

当然，调试模式下是可以定义断点的，不然的话每一行都去单步调试就会十分痛苦。（断点之所以被叫做断点，是因为运行到它们的时候，运行就会停止下来。因此，你可以利用断点跳过自己不感兴趣的代码区域）。请查阅`:h :breakadd`、 `:h :breakdel`和 `:h :breaklist`获取更多细节。

假设你需要知道你每次在保存一个文件的时候有哪些代码在运行：

```vim
:au BufWritePost
" signify  BufWritePost
"     *         call sy#start()
:breakadd func *start
:w
" Breakpoint in "sy#start" line 1
" Entering Debug mode.  Type "cont" to continue.
" function sy#start
" line 1: if g:signify_locked
>s
" function sy#start
" line 3: endif
>
" function sy#start
" line 5: let sy_path = resolve(expand('%:p'))
>q
:breakdel *
```

正如你所见，使用`<cr>`命令会重复之前的调试命令，也就是在该例子中的`s`命令。

`:debug`命令可以和[verbose](#verbosity)选项一起使用。


# 杂项



## 常见问题

### 持续粘贴（为什么我每次都要设置 'paste' 模式）

持续粘贴模式让终端模拟器可以区分输入内容与粘贴内容。

你有没有遇到过往 Vim 里粘贴代码之后被搞的一团糟？

这在你使用 `cmd+v`、`shirt-insert`、`middle-click` 等进行粘贴的时候才会发生。
因为那样的话你只是向终端模拟器扔了一大堆的文本。
Vim 并不知道你刚刚是粘贴的文本，它以为你在飞速的输入。
于是它想缩进这些行但是失败了。

这明显不是个问题，如果你用 Vim 的寄存器粘贴，如：`"+p` ，这时 Vim 就知道了你在粘贴，就不会导致格式错乱了。

使用 `:set paste` 就可以解决这个问题正常进行粘贴。见 `:h 'paste'` 和 `:h 'pastetoggle'` 获取更多信息。

如果你受够了每次都要设置 `'paste'` 的话，看看这个能帮你自动设置的插件：[bracketed-paste](https://github.com/ConradIrwin/vim-bracketed-paste)。

[点此](http://cirw.in/blog/bracketed-paste)查看该作者对于这个插件的更多描述。

Neovim 尝试把这些变得更顺畅，如果终端支持的话，它会自动开启持续粘贴模式，无须再手动进行切换。

### 在终端中按 ESC 后有延时

如果你经常使用命令行，那么肯定要接触 _终端模拟器_ ，如 xterm、gnome-terminal、iTerm2 等等（与实际的[终端](https://en.wikipedia.org/wiki/Computer_terminal)不同）。

终端模拟器与他们的祖辈一样，使用 [转义序列](https://zh.wikipedia.org/wiki/%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97) （也叫 _控制序列_ ）来控制光标移动、改变文本颜色等。转义序列就是以转义字符开头的 ASCII 字符串（用[脱字符表示法](https://zh.wikipedia.org/wiki/%E8%84%B1%E5%AD%97%E7%AC%A6%E8%A1%A8%E7%A4%BA%E6%B3%95)表示成 `^[` ）。当遇到这样的字符串后，终端模拟器会从[终端信息](https://en.wikipedia.org/wiki/Terminfo)数据库中查找对应的动作。

为了使用问题更加清晰，我会先来解释一下什么是映射超时。在映射存在歧义的时候就会产生映射超时：

```vim
:nnoremap ,a :echo 'foo'<cr>
:nnoremap ,ab :echo 'bar'<cr>
```

上面的例子中两个映射都能正常工作，但是当输入 `,a` 之后，Vim 会延时 1 秒，因为它要确认用户是否还要输入那个 `b`。

转义序列会产生同样的问题：

- `<esc>` 作为返回普通模式或取消某个动作的按键而被大量使用
- 光标键使用转义序列进行的编码
- Vim 期望 <kbd>Alt</kbd> （也叫作 _Mate Key_ ）会发送一个正确的 8-bit 编码的高位，但是许多终端模拟器并不支持这个（也可能默认没有启用），而只是发送一个转义序列作为代替。

你可以这样测试上面所提到的事情： `vim -u NONE -N` 然后输入 `i<c-v><left>` ，你会看到一个以 `^[` 开头的字符串，表明这是一个转义序列，`^[` 就是转义字符。

简而言之，Vim 在区分录入的 `<esc>` 和转义序列的时候需要一定的时间。

默认情况下，Vim 用 `:set timeout timeoutlen=1000`，就是说它会用 1 秒的时间来区分有歧义的映射 _以及_ 按键编码。这对于映射来说是一个比较合理的值，但是你可以自行定义按键延时的长短，这是解决该问题最根本的办法：

```vim
set timeout           " for mappings
set timeoutlen=1000   " default value
set ttimeout          " for key codes
set ttimeoutlen=10    " unnoticeable small value
```

在 `:h ttimeout` 里你可以找到一个关于这些选项之间关系的小表格。

而如果你在 tmux 中使用 Vim 的话，别忘了把下面的配置加入到你的 `~/.tmux.conf`文件中：

    set -sg escape-time 0
