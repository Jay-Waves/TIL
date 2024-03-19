``
- 在 Bash 中，变量有许多的扩展方式。`${name:?error message}` 用于检查变量是否存在。此外，当 Bash 脚本只需要一个参数时，可以使用这样的代码 `input_file=${1:?usage: $0 input_file}`。在变量为空时使用默认值：`${name:-default}`。如果你要在之前的例子中再加一个（可选的）参数，可以使用类似这样的代码 `output_file=${2:-logfile}`，如果省略了 `$2`，它的值就为空，于是 `output_file` 就会被设为 `logfile`。数学表达式：`i=$(( (i + 1) % 5 ))`。序列：`{1..10}`。截断字符串：`${var%suffix}` 和 `${var#prefix}`。例如，假设 `var=foo.pdf`，那么 `echo ${var%.pdf}.txt` 将输出 `foo.txt`。

- ssh 中，了解如何使用 `-L` 或 `-D`（偶尔需要用 `-R`）开启隧道是非常有用的，比如当你需要从一台远程服务器上访问 web 页面。

- 对 ssh 设置做一些小优化可能是很有用的，例如这个 `~/.ssh/config` 文件包含了防止特定网络环境下连接断开、压缩数据、多通道等选项：
```
      TCPKeepAlive=yes
      ServerAliveInterval=15
      ServerAliveCountMax=6
      Compression=yes
      ControlMaster auto
      ControlPath /tmp/%r@%h:%p
      ControlPersist yes
```

- 考虑使用 [`mosh`](https://mosh.org/) 作为 ssh 的替代品，它使用 UDP 协议。它可以避免连接被中断并且对带宽需求更小，但它需要在服务端做相应的配置。

- 使用 [`percol`](https://github.com/mooz/percol) 或者 [`fzf`](https://github.com/junegunn/fzf) 可以交互式地从另一个命令输出中选取值。

- 使用 `fpp`（[PathPicker](https://github.com/facebook/PathPicker)）可以与基于另一个命令(例如 `git`）输出的文件交互。

- 将 web 服务器上当前目录下所有的文件（以及子目录）暴露给你所处网络的所有用户，使用：
`python -m SimpleHTTPServer 7777` （使用端口 7777 和 Python 2）或`python -m http.server 7777` （使用端口 7777 和 Python 3）。

- 了解命令行的 [128K 限制](https://wiki.debian.org/CommonErrorMessages/ArgumentListTooLong)。使用通配符匹配大量文件名时，常会遇到“Argument list too long”的错误信息。（这种情况下换用 `find` 或 `xargs` 通常可以解决。）


## 文件及数据处理


- 了解如何使用 `tee` 将标准输入复制到文件甚至标准输出，例如 `ls -al | tee file.txt`。

- 使用 [`repren`](https://github.com/jlevy/repren) 来批量重命名文件，或是在多个文件中搜索替换内容。（有些时候 `rename` 命令也可以批量重命名，但要注意，它在不同 Linux 发行版中的功能并不完全一样。）
```sh
      # 将文件、目录和内容全部重命名 foo -> bar:
      repren --full --preserve-case --from foo --to bar .
      # 还原所有备份文件 whatever.bak -> whatever:
      repren --renames --from '(.*)\.bak' --to '\1' *.bak
      # 用 rename 实现上述功能（若可用）:
      rename 's/\.bak$//' *.bak
```

- 根据 man 页面的描述，`rsync` 是一个快速且非常灵活的文件复制工具。它闻名于设备之间的文件同步，但其实它在本地情况下也同样有用。在安全设置允许下，用 `rsync` 代替 `scp` 可以实现文件续传，而不用重新从头开始。它同时也是删除大量文件的[最快方法](https://web.archive.org/web/20130929001850/http://linuxnote.net/jianingy/en/linux/a-fast-way-to-remove-huge-number-of-files.html)之一：
```sh
mkdir empty && rsync -r --delete empty/ some-dir && rmdir some-dir
```

- 若要在复制文件时获取当前进度，可使用 `pv`，[`pycp`](https://github.com/dmerejkowsky/pycp)，[`progress`](https://github.com/Xfennec/progress)，`rsync --progress`。若所执行的复制为block块拷贝，可以使用 `dd status=progress`。

- 操作日期和时间表达式，可以用 [`dateutils`](http://www.fresse.org/dateutils/) 中的 `dateadd`、`datediff`、`strptime` 等工具。 

- 文件属性可以通过 `chattr` 进行设置，它比文件权限更加底层。例如，为了保护文件不被意外删除，可以使用不可修改标记：`sudo chattr +i /critical/directory/or/file`

- 使用 `getfacl` 和 `setfacl` 以保存和恢复文件权限。例如：
```sh
   getfacl -R /some/path > permissions.txt
   setfacl --restore=permissions.txt
```

- 为了高效地创建空文件，请使用 `truncate`（创建[稀疏文件](https://zh.wikipedia.org/wiki/稀疏文件)），`fallocate`（用于 ext4，xfs，btrf 和 ocfs2 文件系统），`xfs_mkfile`（适用于几乎所有的文件系统，包含在 xfsprogs 包中），`mkfile`（用于类 Unix 操作系统，比如 Solaris 和 Mac OS）。

## 系统调试


- 查找正在使用带宽的套接字连接或进程，使用 [`iftop`](http://www.ex-parrot.com/~pdw/iftop/) 或 [`nethogs`](https://github.com/raboof/nethogs)。

- `ab` 工具（Apache 中自带）可以简单粗暴地检查 web 服务器的性能。对于更复杂的负载测试，使用 `siege`。`ab` 或 [`wrk`](https://github.com/wg/wrk)：web 服务器性能分析

- 了解 `strace` 和 `ltrace`。这俩工具在你的程序运行失败、挂起甚至崩溃，而你却不知道为什么或你想对性能有个总体的认识的时候是非常有用的。注意 profile 参数（`-c`）和附加到一个运行的进程参数 （`-p`）。

- 关于更深层次的系统分析以及性能分析，看看 `stap`（[SystemTap](https://sourceware.org/systemtap/wiki)），[`perf`](https://en.wikipedia.org/wiki/Perf_(Linux))，以及[`sysdig`](https://github.com/draios/sysdig)。


## 冷门但有用

- `m4`：简单的宏处理器

- `env`：执行一个命令（脚本文件中很有用）

- `printenv`：打印环境变量（调试时或在写脚本文件时很有用）

- `look`：查找以特定字符串开头的单词或行

- `pr`：将文本格式化成页／列形式

- `fold`：包裹文本中的几行

- `column`：将文本格式化成多个对齐、定宽的列或表格

- `expand` 和 `unexpand`：制表符与空格之间转换

- `nl`：添加行号

- `seq`：打印数字

- `toe`：terminfo 入口列表

- [`slurm`](https://github.com/mattthias/slurm)：网络流量可视化

- `dd`：文件或设备间传输数据

- `stat`：文件信息

- `logrotate`： 切换、压缩以及发送日志文件

- `watch`：重复运行同一个命令，展示结果并／或高亮有更改的部分

- [`when-changed`](https://github.com/joh/when-changed)：当检测到文件更改时执行指定命令。参阅 `inotifywait` 和 `entr`。

- `strings`：从二进制文件中抽取文本

- `iconv` 或 `uconv`：文本编码转换

- `split` 和 `csplit`：分割文件

- `sponge`：在写入前读取所有输入，在读取文件后再向同一文件写入时比较有用，例如 `grep -v something some-file | sponge some-file`

- `units`：将一种计量单位转换为另一种等效的计量单位（参阅 `/usr/share/units/definitions.units`）

- `nm`：提取 obj 文件中的符号


- [`mtr`](http://www.bitwizard.nl/mtr/)：更好的网络调试跟踪工具

- `cssh`：可视化的并发 shell


- [`wireshark`](https://wireshark.org/) 和 [`tshark`](https://www.wireshark.org/docs/wsug_html_chunked/AppToolstshark.html)：抓包和网络调试工具

- [`ngrep`](http://ngrep.sourceforge.net/)：网络层的 grep

- `host` 和 `dig`：DNS 查找

- `lsof`：列出当前系统打开文件的工具以及查看端口信息

- [`glances`](https://github.com/nicolargo/glances)：高层次的多子系统总览

- `last`：登入记录

- [`sar`](http://sebastien.godard.pagesperso-orange.fr/)：系统历史数据

- [`iftop`](http://www.ex-parrot.com/~pdw/iftop/) 或 [`nethogs`](https://github.com/raboof/nethogs)：套接字及进程的网络利用情况

- `sysctl`： 在内核运行时动态地查看和修改内核的运行参数

- `hdparm`：SATA/ATA 磁盘更改及性能分析

- `lsblk`：列出块设备信息：以树形展示你的磁盘以及磁盘分区信息

- `lshw`，`lscpu`，`lspci`，`lsusb` 和 `dmidecode`：查看硬件信息，包括 CPU、BIOS、RAID、显卡、USB设备等

- `lsmod` 和 `modinfo`：列出内核模块，并显示其细节

- `fortune`，`ddate` 和 `sl`：额，这主要取决于你是否认为蒸汽火车和莫名其妙的名人名言是否“有用”


## 更多资源

- [awesome-shell](https://github.com/alebcay/awesome-shell)：一份精心组织的命令行工具及资源的列表。
- [awesome-osx-command-line](https://github.com/herrbischoff/awesome-osx-command-line)：一份针对 OS X 命令行的更深入的指南。
- [Strict mode](http://redsymbol.net/articles/unofficial-bash-strict-mode/)：为了编写更好的脚本文件。
- [shellcheck](https://github.com/koalaman/shellcheck)：一个静态 shell 脚本分析工具，本质上是 bash／sh／zsh 的 lint。
- [Filenames and Pathnames in Shell](http://www.dwheeler.com/essays/filenames-in-shell.html)：有关如何在 shell 脚本里正确处理文件名的细枝末节。
- [Data Science at the Command Line](http://datascienceatthecommandline.com/#tools)：用于数据科学的一些命令和工具，摘自同名书籍。
