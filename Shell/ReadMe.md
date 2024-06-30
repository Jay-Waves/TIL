
## Bash 工具列表

[安全与权限](Bash/安全与权限.md)

| `chmod` | `chown` |
| ------- | ------- |
| `getfacl`        |         |

[调试及技巧](Bash/调试及技巧.md)

| `ldd`    |  | `dmesg` | `strace, ltrace` |
| -------- | ------ | ------- | ---------------- |
| `nm`     | `stap` | `stap`  | `perf`           |
| `sysdig` | `time` | `hyperfine`        |                  |

[进程管理](Bash/进程管理.md)

| `pkill`  | `pgrep`   | `pstree` | `lsof`          | `(h)top`    |
| -------- | --------- | -------- | --------------- | --- |
| `fg, bg` | `jobs`    | `ps`     | `kill, killall` | `&` |
| `nohup`  | `timeout` |     |                 |     |
| `watch`  | `inotifywait`          |          |                 |     |

[配置及帮助](Bash/配置及帮助.md)

| `man`     | `type`  | `whatis` |
| --------- | ------- | -------- |
| `whereis` | `which` |          |

[网络](Bash/网络.md)

| `ip`                | `ping`      | `mtr`   | `netcat, socat` |
| ------------------- | ----------- | ------- | --------------- |
| `nslookup`          | `dig, host` | `whois` |                 |
| `netstat, ss`           | `slurm`        | `iftop` | `nethogs`                |
| `wireshark, tshark` |             |         |                 |
| `scp`               | `rsync`     | `ssh`   |                 |
| `curl`              | `wget`      |         |                 |
| `unshare`           | `firejail`  |         |                 |

[文本处理](Bash/文本处理.md)

| `pandoc`           | `jq`                 | `shyaml`        | `xmlstarlet`   | `csvkit` |
| ------------------ | -------------------- | --------------- | -------------- | -------- |
| `hd, hexdump, xxd` | `hexedit, biew, bvi` | `(c)split`      | `strings`      |          |
| `awk`              | `sed`                | `grep, ripgrep` | `tokei`               |          |
| `cut, paste, join` | `echo, printf`       | `fmt`           |                |          |
| `uniq`             | `sort`               | `nl`            | `iconv, uconv` | `expand` |
| `cat, bat`         | `diff`               | `tail, head`    | `more, less`   |          |

[文件系统](Bash/文件系统.md)

|            | `mount`        | `fdisk`  | `mkfs`   | `lsblk` |
| ---------- | -------------- | -------- | -------- | ------- |
| `cd`       | `ls, tree`     | `mkdir`  | `pwd`    | `ln`    |
| `df`       | `du`           | `quota`  | `ldparm` |         |
| `find, fd`     | `locate`       |  `zoxide`        |          |         |
| `mv`       | `cp`           | `repren` | `rm`     | `touch` |
| `truncate` | `when changed` |          |          |         |
| `stat`     | `file`         | `chattr` | `wc`     |         |
| `xz`       | `7zip`         | `tar`    | `gzip`         |         |

[系统信息采集](Bash/系统信息采集.md)

| `/proc`    | | `w`      | `uname` | `sysstat, dstat` |
| ---------- | -------- | -------- | ------- | ---------------- |
| `free`     | `lsxxx`  |          |         |                  |
| `screen`   | `tmux`   |          |         |                  |
| `shutdown` | `halt`   | `reboot` |         |                  |
| `uptime`   | `dateutils`         |          |         |                  |

[用户管理](Bash/用户管理.md)

| `finger`   | `whoami`  | `last` | `passwd` | `id` |
| ---------- | --------- | ------ | -------- | ---- |
| `sudo, su` | `usermod` | `exit` | `groupmod`         |      |

[Bash 脚本编程](Bash/编程/1%20Variables.md)

| `export` | `env`   |     |     |     |
| -------- | ------- | --- | --- | --- |
| `tee`    | `xargs` | `history`    |     |     |

## 参考

- [awesome-shell](https://github.com/alebcay/awesome-shell) 一份工具列表.
- [awesome-osx-command-line](https://github.com/herrbischoff/awesome-osx-command-line): 一份针对 osX 的工具列表
- [Strict mode](http://redsymbol.net/articles/unofficial-bash-strict-mode/): bash 编程指导
- [shellcheck](https://github.com/koalaman/shellcheck): 静态 Shell 脚本分析工具, 本质是 bash/sh/zsh 的 lint
- [Filenames and Pathnames in Shell](http://www.dwheeler.com/essays/filenames-in-shell.html): 如何在脚本中正确处理文件名.
- [Data Science at the Command Line](http://datascienceatthecommandline.com/#tools)
