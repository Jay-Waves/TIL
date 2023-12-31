## 网络防火墙

通过一系列设定的规则, 过滤网络流量 (数据包). [参考](https://zhuanlan.zhihu.com/p/461078581)

### 防火墙类型

- 分组过滤防火墙: 通过限定 *网络地址, 协议, 端口*, 过滤 TCP/UDP, IP 报文.
- 状态防火墙: 基于历史连接, 检查当前数据包是否是"已建立, 已批准"的.
- 应用层防火墙: 过滤 HTTP, FTP 等特定协议.
- 代理防火墙: 作为客户端和服务器间的中介.

### 防火墙接口模式

- L3: NAT 模式, 类似路由器接口, 拥有 IP. 可静态配置 IP, 也可通过 DHCP 动态获取.
- L2: 透传(透明)模式, 类似交换机. 使用 VLAN 分配 IP.
- L1: 虚拟线缆, 将两个接口一个作为输入一个作为输出.
- TAP: 与交换机镜像端口连接的模式. 仅能检测, 无法阻止.

L1-3 是串联在网路上, 而 TAP 是旁挂在交换机上.

### 防火墙功能

- *会话管理*: 防范 Dos (Denial of Service) 攻击, 控制负载.
- *报文结构解析*: 防范非法报文.

**安全区域与安全策略:**

- *安全区域*: 设置 Trust Zone, Untrust Zone, DeMilitarized Zone (DMZ). 跨安全区域通信是默认拒绝的 (Implicit Deny)

- *安全策略*: 维护访问控制列表 access list, 但是以 Zone 为单位进行控制. 内容安全策略包括: *反病毒*, *IPS*(Intrusion Prevention System)、*URL 过滤*、*DLP*(数据泄露防护) 等基于内容的安全机制.

#### 提供 NAT
NAT (Network Address Translator), 将私有 IP 转化为公网 IP. 运行 nat 的路由器或防火墙叫做网关 (Gateway). 静态 nat 一对一映射 IP, 动态 nat 在网关内部有 IP 池. nat 还能节省公网 IP.

#### 提供 VPN:

Virtual Private Network. 通过公有网络, 搭建私有内部网络, 保密信息.
- site-to-site VPN: 点对点的建立 IPsec 隧道.
- hub and spoke VPN: 星型拓扑结构.
- remote VPN: 通过 vpn 软件连接到 VPN 总节点, 通过总节点和其他分节点交流. 除了网络层实现 IPsec-VPN, 还有应用层 SSL-VPN. SSL-VPN 通过 https (http over SSL) 在 Web 远程接入 VPn, 因为使用的协议是 https, 所以基本不会被防火墙拦截; 但是支持的报文类型少.

VPN-IPsec 建立过程: `PCA` -> `GateWayA` -> `IPsec` -> `GateWayB` -> `PCB`
1. `PCA` 发送请求到 `GateWayA`, (即是网关也是, 也是 VPN 设备)
2. `GateWayA` 加密报文, 添加 ESP 头部, 和在 IPsec 隧道中使用的伪装 IP 头部.
3. `GateWayB` 收到密文, 检查 ESP 和 AH 头部是否合规.
4. `GateWayB` 确认无通信异常后, 解密, 去除冗余头部, 按原 `PCa` 报文进行路由, 到达 `PCB`

#### DoS 防御:

DDos (Distributed Denial of Sevice) 通过僵尸网络的多个跳板, 对服务器发起攻击.

**攻击类型:**
- Syn Flood: 发送大量 TCP SYN 报文, 使服务器无法建立 tcp 连接.
- ICMP Flood: 发送大量 ICMP echo request.
- UDP/IP Flood: 发送大量 udp, 占用内存.
- Land: 发送目的地址和源地址相同的报文, 使服务器不断向自己发送数据.
- Tear Drop: 发送伪造的, 含有假 offset 的 tcp 报文.
- Ping of Death: 超过 IP 最大长度 65535 的 ping.
- Smurf: 广播源地址为攻击目标的 ICMP echo request, 使攻击目标收到大量 ICMP echo reply.
- Fraggle: 类似 Smurf, 但使用 UDP.
- Connection Flood: 开启大量连接, 不释放.
- Reload: 不断请求网页重载 (浏览器 F5).

**防御 Dos:**

限制异常高速的通信流量.

#### 报文攻击防御

非法报文攻击：
- IP 地址欺骗 (IP Spoofing): 伪造 IP 源地址, 躲避日志记录.
- 伪装为 IP/ICMP 分片报文
- 巨型 ICMP 报文
- TCP 非法控制报文

**IPS/IDS:**
- IDS: Intrusion Detection System, 检测非法入侵, 记录日志, 并告知系统管理员.
- IPS: Intrusion Prevention System, 通过协议和程序拦截非法入侵, 并反击(伪装).

IPS/IDS 能检测:
- Dos 攻击.
- P2P 造成的信息泄露.
- 运行蠕虫, 特洛伊木马, 键盘记录器等恶意软件.
- 入侵局域网和入侵侦查行为

> CVE (Common Vulnerabilities Exposures) 是美国非盈利机构 MIRTE 发起的收集已知漏洞的项目. 编号 (CVE-ID): `CVE-year-no`.

## ufw 使用

- `sudo ufw enable`
- `sudo ufw disable`

规则:
- `sudo ufw allow ssh`
- `sudo ufw allow 22/tcp` 允许端口 22 的 tcp 报文.
- `sudo ufw allow from 192.168.1.10 to any port 22`
- `sudo ufw deny 1234/tcp`
- `sudo ufw delete [rule_number]`, rule_number 通过 `status` 命令查看.