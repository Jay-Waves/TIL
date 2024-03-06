## IPv4

### IP地址分类

开头为哈夫曼编码, 对IP进行唯一区分

- A类地址, 默认`Net ID`占8bits, `Host ID`占24bits, 开头固定为 `0`

- B类地址, 默认`Net ID`占16bits, `Host ID`占16bits, 开头固定为 `10`

- C类地址, 默认`Net ID`占24bits, `Host ID`占8bits, 开头固定为 `110`

- D类地址, 开头固定为 `1110`, 用于多播地址

- E类地址, 开头固定为 `1111`, 保留今后使用

#### 特殊IP:

看网络段和主机段, 任意为全0或全1, 则为特殊地址.  
全0泛指网络地址, 1泛指广播地址  

127开头: 环回地址

#### 私有IP:

- A类: `Net ID`为10
- B类: `Net ID`为172.16 - 172.31
- C类: `Net ID`为192.168.0 - 192.168.255

### IP数据报

首部前20字节固定长, 其后长度可变.

```
  0                   1                   2                   3   
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |  Ver  |  IHL  |Type of Service|          Total Length         |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |        Identification         |Flags|       Fragment Offset   |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |  Time to Live  |   Protocol   |       Header Checksum         |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                        Source IP Address                      |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                     Destination IP Address                    |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                             Options             |   padding   |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                             Data                              |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
- Version: 版本号 (IPv4: 4, IPv6: 6)
- IHL (Internet Header Length):  IP头长度, 单位 4 bytes.
- Type of Service: usually unused
- Total Length: 数据+头部总长度, 单位 bytes.
- Identification: 用于分片, 标识同一数据的分片
- Flags: reserved|DF|MF, DF: don't fragment 不分片, MF: more fragment 后续还有改报文的分片.
- Fragment Offset: 分片原偏移, 单位 8 bytes
- Time to Live: 每转发一次, 减一.
- Protocol: 数据是哪个上层协议负责处理的, 比如 ICMP/OSPF/TCP/UDP 等.
- Options: 可扩展功能段

## IPv6

ipv6 和 ipv4 是不兼容的, 这导致了从 ipv4 向 ipv6 过渡困难.

## 应用

### 开启 IPv4 转发

临时开启: `sudo sysctl -w net.ipv4.ip_forward=1`

永久开启: 

- 修改 `/etc/sysctl.conf`, 添加 `net.ipv4.ip_forward = 1`
- 重启服务 `sudo sysctl -p`

