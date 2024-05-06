
## 关机操作流程

1. 使用 `who` 查看主机是否有还在线用户
2. 使用 `netsat -a` 确定是否有网络连接
3. 使用 `ps -aux` 查看后台进程状态
1. 使用数据同步 `sync`
2. 关机

## 常用命令:
- `shutdown -h 30` 30分钟后自动关机
- `shutdown -h now` 立刻关机(root)
- `shutdown -r now` 立刻重启(root)
- `shutdown -r 00:30` 在00:30时候重启(root)
- `init 0` 也可以关机, `init 6`也是重启