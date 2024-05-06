   Arch 系统比较轻量和灵活, 需要用户自行配置决定.

## 初始化 Arch

使用 Virtual Box 临时启动 iso, 此时是内存中的一个最小化**临时**系统. 此时, 关闭就会丢失所有数据. 我们要手动将系统从 iso 正式安装到硬盘上 (Ubuntu 这里会有安装引导, Arch 自己搞, 能学到很多).

### 网络

同时开启两个网卡, 分别为 Host-Only 和 Nat 模式.

Virtual Box 默认在 Windows 上的以太网接口为 192.168.56.1. NAT 模式为虚拟机分配一个隔离子网地址, 如 10.0.x.x, 用于通过网关访问 Internet; Host-Only 模式分配本地地址, 如 192.168.56.x , 仅用于主机和虚拟机间通信.

若无法 ping 通本机网关, 但能 ping 通主机以太网接口, 应该是 windows 防火墙问题. 添加一个入站规则, 允许虚拟机 ip (通过 `ip addr` 查看)通过. 原理是, 防火墙会可能拦截该 DHCP 报文(动态主机配置报文), dhcp 用 ip 数据报传输, 被拦截导致本地网络无法正常配置.

**关于DNS:**

域名解析不出来. `nslookup` 显示的域名服务器可能是 127.0.0.53, 一些 linux 系统会监听这个地址, dns请求报文发送到该地址, 然后被 linux 转发给真正的 dns 解析服务器. 使用 `systemd-resolve --status` 查看真实 dns.

**关于VPN:**

虚拟机配置 `export ALL_PROXY="http://192.168.56.1:7890"` 转发报文到代理客户端. 当然也能设置为主机 IP, 但是主机 IP 会随网络路由器动态改变. 比如 buaa-wifi 和 buaa-free-wifi 下, 主机分配的 IP 就不一样.

以 Clash 为例, 需打开 Lan 模式, 对本地其他网络串口的 7890 端口进行监听.

> 详见 [代理](../../../Network/防火墙/代理.md)

**测试连接状态:**

别使用 ping 了, 大部分服务器都不会响应 icmp 报文. 可以使用 curl, 直接用 http 协议请求网站响应.

==一定要确保能连接到网络, 才继续下一步安装操作==

### 安装

**文件系统:**

硬盘分区: fdisk 是分区工具.
1.  `fdisk /dev/sda`
2.  `n` -- new partition
3.  (enter) (`p` -- primary disk)
4.  (enter) (`1` -- partition number)
5.  (enter) set first sector
6.  (enter) set last sector (use whole disk)
7.  `w` -- write partition table and quit

在分区上创建 ext4 文件系统:
`mkfs.ext4 /dev/sda1`

挂载文件系统:
`mount /dev/sda1 /mnt`

安装 arch base (基础)程序包以及 linux 内核.
`pacstrap /mnt base linux`  

Generate `/etc/fstab`:  fstab: file system table, 定义系统启动时如何挂载文件系统和存储设备
`genfstab -U /mnt >>/mnt/etc/fstab`

Change root to the newly created system:  
`arch-chroot /mnt`

**时区及语言:**

Set the timezone, e.g.:  设定时区
`ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime`

Generate `/etc/adjtime`: 将系统时间与硬件时间同步
`hwclock --systohc`

Enable locale:

1.  Uncomment your preferred locale (e.g. `en_US.UTF-8 UTF-8`) in `/etc/locale.gen` using `vi`.
2.  Run `locale-gen`.
3.  Put the corresponding `LANG` variable (e.g. `LANG=en_US.UTF-8`) in `/etc/locale.conf`:  
    `echo 'LANG=en_US.UTF-8' >/etc/locale.conf`

中文支持: 

locale.gen 中取消注释 `zh_CN.UTF-8`, 然后 `$ sudo locale-gen`. 不要将 LANG 和 LC_ALL 设置为中文, 因为默认 linux 终端中无法渲染 (除非内核打了补丁), 并且中文会导致 pacman 的源变为镜像.

**用户注册:**

Set hostname (e.g. `my_laptop`):  
`echo my_laptop >/etc/hostname`

Enable DHCP client daemon:  启动 dhcp 守护进程. `systemctl` 配置系统服务, `enable` 设置为自启动. 守护进程后缀: `.d`, `.conf`
`systemctl enable dhcpcd`

Set root password:  
`passwd`  

Install GRUB: 启动引导程序, 64位版需要 efi, 否则只能用32位 bios启动 (启动速度慢点, 别的没差).

1.  `pacman -S grub`
2.  `grub-install --target=i386-pc /dev/sda`
3.  `grub-mkconfig -o /boot/grub/grub.cfg`

Reboot:

1.  `exit`
2.  `umount -R /mnt`
3.  `reboot`

Now boot from the first hard disk.

修改 hostname:

```bash
hostnamectl set-hostname <new-hostname>

vim /etc/hostname
vim /etc/hosts

# then reboot
```

**常用工具:**

`net-tools`

### 添加新用户

比如添加一个用户从 AUR 上下载野包的用户 software

`useradd -m -s /bin/bash software`

`passwd software`

`usermod -aG wheel software` 

将 software 用户添加到 sudo 用户组: `EDITOR=vim visudo`, 取消注释 `%wheel ALL=(ALL) ALL`. (只有 wheel 组用户可以用 su 变为 root)

将 bash 改为 zsh: `sudo chsh -s /usr/bin/zsh software`

默认 root 没有密码, 即不能通过 `su` 命令进入. 用 `sudo passwd root` 配置密码后, 能够完全进入 root 身份.

> 参考: 
> 
> [简易 Arch 安装](https://gist.github.com/thomasheller/5b9b18917bbaabceb4f629b793428ee2)