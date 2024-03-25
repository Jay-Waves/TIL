### 1 下载源码

查看当前 linux kernel 版本: `uname -r`

下载 github 发布的 WSL2 内核 `wget https://github.com/microsoft/WSL2-Linux-Kernel/archive/refs/tags/linux-msft-wsl-<version>.tar.gz`, 详见 [github release](https://github.com/microsoft/WSL2-Linux-Kernel/releases)

或者直接从官网下载[Linux官方发行版](https://www.kernel.org/)

### 2 安装依赖

`sudo apt install build-essential flex bison libssl-dev libelf-dev`

可能还需要:

`sudo apt-get install libncurses5-dev libncursesw5-dev`

`sudo apt-get install bc`

### 3 编译源码

进入源码目录

需要为编译文件夹创建一个配置文件`.config`, 用来适配WSL. 
该配置文件可以从官方GithubRelease中[`Microsoft/config-wsl`](https://github.com/microsoft/WSL2-Linux-Kernel/blob/linux-msft-wsl-5.15.y/Microsoft/config-wsl)获取, 实际上就在源码中: `arch/x86/configs/config-wsl`

**编译 `sudo make -j 4`, j4指开四个线程编译**  
一般linux编译可能还需要步骤:  
- `sudo make modules`
- `sudo make modules_install`
- `sudo make install`

> 可替换命令
> `sudo make KCONFIG_CONFIG=Microsoft/config-wsl`

### 4 更换内核

编译好内核在当前目录下的`arch\x86\boot\bzimage`,  
将其放入windows用户文件夹:`c:\\users\\yjw`

在用户文件夹的配置文件`.wslconfig`中, 写入

```
[wsl2]
kernel=c:\\Users\\yjw\\bzImage
```

关闭wsl, `wsl --shutdown`

### 参考

> [How to build and use a kernel in WSL2 ⋅ Plume](https://bashell.com/~/Cwt/how-to-build-and-use-a-kernel-in-wsl2)

> [如何让WSL2使用自己编译的内核 - 知乎](https://zhuanlan.zhihu.com/p/324530180)