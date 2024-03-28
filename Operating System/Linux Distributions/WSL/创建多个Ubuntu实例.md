```shell
wsl --export Ubuntu-22.04 D:\VM\backup.tar.gz
wsl --import Ubuntu-22.04-2 <path\to\install> D:\VM\backup.tar.gz

# 或直接导入 vhdx 文件 (更快)
wsl --import-in-place <Distro> <FilePth>

# login the second instance  
wsl -d Ubuntu-22.04-2
```

## 将已有 Ubuntu 安装到其他盘

核心就是移动 vhdx 文件.

```shell
wsl --export Ubuntu-22.04 D:\VM\backup.vhdx --vhd
wsl --unregister Ubuntu-22.04
wsl --import Ubuntu-22.04 D:\VM\Ubuntu-22.04 D:\VM\backup.vhdx --vhd
wsl --set-default-version Ubuntu-22.04
```