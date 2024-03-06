## 虚拟机配置代理

 1. 开启 vpn 客户端 (如 Clash) 的 LAN 模式

虚拟机 (linux) 和主机 (win10) 组成了一个局域网, 使用 `ipconfig` 在 cmd 中查看. LAN 模式让 clash 能够作为 proxy 转发局域网内的报文至真正的 proxy for vpn. 

2. 配置 proxy 环境变量, 可以写在 bashrc 中

clash 在局域网内仅接收 http 协议转发报文, 并且端口为 7890. 

clash 不接收 https 协议转发的报文, 因为本地内没有验证必要; 也别使用如 socks5 这种应用层之下的协议, 没有交互/错误处理/证书验证等能力, 容易出错, 至少我的虚拟机上会出现 TLS 验证问题.

host_ip 指的是主机 (win10) 的 ip. 我们将报文转发到该地址. **如果频繁更换 wifi (如一会 buaa wifi, 一会 buaa free wifi, 学校路由器会用 dhcp 从地址池给你分配一个动态地址), 主机的 ip 也是会不断跟着变化的. 更方便的办法是转发到主机上连接虚拟机的那个端口上, 以下是在 cmd 中执行 `ipconfig` 结果, 我将 wsl 虚拟机的以太网 ipv4 地址作为虚拟机 proxy 啦.**

```
无线局域网适配器 WLAN:
   连接特定的 DNS 后缀 . . . . . . . :
   IPv6 地址 . . . . . . . . . . . . : 2001:250:206:6cc3:98dc:5689:9402:aa67
   临时 IPv6 地址. . . . . . . . . . : 2001:250:206:6cc3:c13f:f0f:c0e7:1503
   本地链接 IPv6 地址. . . . . . . . : fe80::aa0:c9ce:8092:1a67%16
   IPv4 地址 . . . . . . . . . . . . : 10.195.208.xxx
   子网掩码  . . . . . . . . . . . . : 255.255.0.0
   默认网关. . . . . . . . . . . . . : fe80::b244:14ff:fe6a:c802%16
                                       10.195.0.1

以太网适配器 vEthernet (WSL):

   连接特定的 DNS 后缀 . . . . . . . :
   本地链接 IPv6 地址. . . . . . . . : fe80::b3ec:25ef:182f:cbd6%67
   IPv4 地址 . . . . . . . . . . . . : 172.18.240.1
   子网掩码  . . . . . . . . . . . . : 255.255.240.0
   默认网关. . . . . . . . . . . . . :
```

```bash
proxy="host_ip:host_port"
export $ALL_PROXY="http://$proxy"
```

不同软件针对不同协议检查的 proxy 环境变量不一样, 还可能是 `http_proxy` `https_proxy` `HTTP_PROXY` `HTTPS_PROXY`, 保险可以都设上. 我觉得 `ALL_PROXY` 够用 (区分大小写)

3. 一些软件用的是自己的 proxy, 不是系统的. 比如 go, git, 还有一些隐私浏览器和 IDE. 配置也类似步骤2

```shell
# git proxy
# git 可以使用 socks5, 因为 git 使用自己的 SSL
git config --global http.proxy "socks5://127.0.0.1:1080"
git config --global https.proxy "socks5://127.0.0.1:1080"

# 查看配置
git config --list


# go proxy
# go get 会先尝试使用自己的服务器, 通常被指定为坑爹的 goproxy.cn 实际上访问不了
# go get 使用 direct 时, 才会使用系统 proxy 访问 github 原仓库.
go env -w GOPROXY=https://goproxy.io,direct
```

### gain proxy ip

`netstat -na`


> 参考 [机场+VPN](https://playbeasts.com/question/2691)

## VPN软件

TUN模式: 工作于网络层, 针对点对点IP封装和VPN路由, 非网络层报文不会处理, 节省带宽.

TAP模式: 工作在链路层, 处理以太网帧, 模拟一个以太网网关环境. 

## 内网穿透

在公司外需要用ssh访问公司内网设备, 但内网设备的IP是内网网段.

**1. ssh服务器**

**2. 路由器配置端口转发**
- 打开路由器配置页面, 一般托管在内网网关IP: `https://gateway_ip`
- 配置报文转发: 外部端口设置一个不重复的(10000以上那种), 内部端口为22(ssh), 内部地址为要连接的内网主机.
- 网关设备会根据端口来区分内网主机, 从而完成nat转换.