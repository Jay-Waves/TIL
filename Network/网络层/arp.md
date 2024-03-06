## ARP协议

ARP: address resolution protocol, 地址解析协议.   
用于从IP地址寻得其映射的硬件地址, 寻找方式是向网络中发送ARP请求.

每台主机缓存ARP表, 记录`<IP, MAC, TTL>`映射关系, TTL为有效期.

