如果打不开win-kex, 可以输入命令`sudo mount -o remount,rw /tmp/.X11-unix`
具体原理尚不清楚, 大概是代理的问题.

kex 三种模式:
1. 桌面模式: `kex --win`
2. 增强会话模式: `kex --esm --ip`, 更高分辨率, 更快的链接, 更多的资源消耗
3. 无缝模式: `kex --sl` windows任务栏在屏幕下方, kali任务栏在屏幕上方, 同一桌面两个系统


退出kex桌面: `kex --win --stop`
增加声音: `-s`, 如 `kex --win -s`
关闭kex后台: `kex kill`

进入窗口后, 摁F8调出菜单. 详见`kex --help
