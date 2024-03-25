- 安装包: `pacman -S <pkg>`
- 安装一组包时, 查看列表: `pacman -Sg <pkgs>` **pacman 会覆盖安装已存在的包, 除非 --needed**

- 删除包: `pacman -R <pkg>`
- 删除包及不再被其他需要的依赖: `pacman -Rs <pkg>`
- 查看孤立包: `pacman -Qdt`, 删除用 `pacman -Rs $(pacman -Qdtq)`

- 检查包是否存在: `pacman -Q <pkg>`
- 列出包存在的位置: `pacman -l <pkg>`, 常用 `-QL`
- 列出更多信息 `pacman -i <pkg>`

- 搜索未知包: `pacman -Ss <pkg>`

### 更新系统

`pacman -Syu`, then reboot.

Arch 是滚动更新的, 并且一次需要更新整个系统, 所有依赖都会被更新至最新版. **一定要避免部分更新, 很可能导致依赖问题.** 

更新前, 应查看[arch 官网](archlinux.org), 查看是否有复杂问题新闻与指导. 更新底层软件 (linux, Systemd, xorg) 时, 需要查看是否有相关 [issue](https://bbs.archlinux.org/).

### FHS

pacman 对软件的管理遵循 FHS (filesystem hierarchy standard)

- `/bin`, `/usr/bin` 用户, 命令可执行文件
- `/sbin`, `/usr/sbin` *系统管理和维护*相关的可执行文件
- `/lib`, `/usr/lib` 动态链接库
- `/etc` 软件的系统级配置文件
- `/opt` 存放 较大或非标准软件
- `/var` 经常变化的数据, 如 `/var/log`, database, email
- `/usr/share` 共享数据, 如 icon, wallpaper, static data
- `/usr/include` 头文件, 用于编译
- `/usr/local` 存放 手动编译安装的软件
- `/home` 个人配置文件