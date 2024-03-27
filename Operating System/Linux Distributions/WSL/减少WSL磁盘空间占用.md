WSL 占用空间后, 如果空间在 WSL 中被释放, 被释放的空间并不会自动归还给 Windows, 造成 WSL 占用磁盘空间不断增长. 解决办法:
1. wsl 新特性 `sparse`, 自动缩减镜像体积 (仅对未来有效)
2. 压缩镜像文件

首先确保最新版: `wsl --update [--pre-release]`

## 设置 `sparse`

通过命令行: `wsl --manage <distro> --set-sparse true`

通过 `.wslconfig`:

```
[experimental]
sparseVhd=true 
```

## 压缩镜像文件

常见方法是通过 Hyper-V 提供的 Optimize-VHD 命令, 但是 Windows Home 版并没有 Hyper-V 功能, 故此处介绍基于 diskpart 的方法.

以 Ubuntu 为例, 虚拟机磁盘文件存在该路径:
```
C:\Users\%username%\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu20.04
onWindows_79rhkp1fndgsc\LocalState\ext4.vhdx
```

使用 diskpart

```powershell
wsl --shutdown
# 检查是否为稀疏文件
fsutil sparse queryflag "....\ext4.vhdx"
# 如是, 就设置为非稀疏文件
fsutil sparse setflag "....\ext4.vhdx" 0

diskpart
# open window Diskpart console
select vdisk file="....\ext4.vhdx"
# 如果 select vdisk file 报错, 可能是路径中有单引号, 去掉双引号即可.
attach vdisk readonly
compact vdisk
detach vdisk
exit
```

## 镜像文件挪窝

可以用 synbolink 软链接把 vhdx 移动到别的盘.

### 参考

- https://github.com/microsoft/WSL/issues/4699
- https://devblogs.microsoft.com/commandline/windows-subsystem-for-linux-september-2023-update/#automatic-disk-space-clean-up-set-sparse-vhd
- [windows 10 - How do I get back unused disk space from Ubuntu on WSL2 - Super User](https://superuser.com/questions/1606213/how-do-i-get-back-unused-disk-space-from-ubuntu-on-wsl2)