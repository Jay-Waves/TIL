## TCP特点

- **面向连接**: 此连接是逻辑连接, 不是物理的
- TCP连接只能是点对点的(一对一), 其端点称为**套接字**, `socket= (IP address: port number). 不提供广播或多播.
- 提供**可靠交付**. 无差错, 不丢失, 不重复, 并按序到达.
- *全双工通信*, TCP在两端均有接收/发送缓存.
- **面向字节流**: tcp将数据视作无结构"字节流", 也不关心其内容, tcp保证字节流内容相同, 但不保证发送/接收报文结构相同

```
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                       Source IP Address                       | <- 伪首部, 用于校验和
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                     Destination IP Address                    |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |       0       |       6       |          TCP Length           |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                
  0               1               2               3   
  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |          Source Port          |       Destination Port        |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                        Sequence Number                        |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                    Acknowledgment Number                      |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |  Data |           |U|A|P|R|S|F|                               |
 | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
 |       |           |G|K|H|T|N|N|                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |           Checksum            |         Urgent Pointer        | 
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                    Options                    |    Padding    | <- 非固定字段
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                             Data                              |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```

1. **Source Port**: 标识发送方应用程序或进程的端口号.
2. **Destination Port**: 标识接收方应用程序或进程的端口号.
3. **Sequence Number**: 整体数据按字节编号, 该字段指明当前报文中数据段的起始编号. 如携带数据 301-400, 则 `SeqNum=301`
4. **Acknowledgment Number**: 指示接收方期望接收的下一个数据段的序列号`N`, 并确认至`N-1`为止所有数据皆收到. 当`Ack`标识为1时有效. 如收到数据 301-400, 则下一报文中 `AckNum=401`.
5. **Data Offset**: 指示TCP报文头部的长度, 以`4bytes`字为单位. 首部**固定(最短)**`20bytes`, 该段长 `4bits`, 因此最大单位是15, 意味着TCP报文首部最长为`60bytes` (固定`20bytes`+选项`40bytes`)
6. **Reserved**: 保留字段, 留作将来使用. 置0.
7. **Control Flags**: 用于标识TCP报文的不同一位控制标志, 如:
	- URG: 紧急标识, `UGR=1`时报文会插队到最前.
	- ACK: `ACK=1`时, AckNum字段才有效. 连接建立后, 必须置1.
	- PSH: 不等待缓存充满, 立即发送或向上交付. 为了优化网络效率, 发送端会等缓冲数据较多时才封装为一个报文发送, 避免浪费, 如: `20bytes`IP首部 + `20bytes`TCP首部 + `1bytes`数据.
	- RST: 严重错误, 须释放连接重新建立.
	- SYN: `SYN=1`时, 表示是连接请求报文.
	- FIn: 用于释放连接.
8. **Window（窗口大小）**: 本报文发送方接收窗口大小, 期望接收方为此连接分配的适宜缓冲区, 用于流量控制.
9. **Checksum（校验和）**: 用于检测TCP报文 ==头部和数据部分== 的完整性.
10. **Urgent Pointer（紧急指针）**: 指示紧急数据的结束位置, 告知TCP何时恢复正常. 须`URG=1`
11. **Options（选项）**: 可选的附加信息, 如:
	- MSS: (Maximum Segment Size) 最大报文段长度, 指**Data**字段最大长度. 为保证传输效率, MSS应尽可能大, 但又要保证IP层不额外对报文切片. 默认`536 bytes`长.
	- 窗口扩大选项: 扩大**Window**字段, ...
	- 时间戳: 计算RTT, 区分循环重复的**SeqNum**.
12. **Padding（填充）**: 保证TCP首部以`4bytes`对齐

## 可靠传输原理

传输可能出现如下差错:
- 误码 (比特差错)
- 分组丢失
- 分组失序
- 分组重复

理想传输条件:
- 信道不产生差错.
- 不管发送速度多快, 接收方总来得及处理收到的数据.

可靠传输协议 (RDT) 的特点: 不错, 不丢, 不乱. 

***

流量控制: 抑制发送端速率, 使接收端来得及处理
拥塞控制: 控制网络输入不超过网络负载

## 流量控制

## 拥塞控制
![|350](../../attach/Pasted%20image%2020230604170719.png)

![|500](../../attach/Pasted%20image%2020230604173012.png)

**慢启动算法**:

```
Initialize ssthesh = ?
Initialize cwnd <- 1 // 1 MSS
For (each segment ACKed):
	cwnd <- cwnd + 1
Until (loss event OR cwnd > ssthresh):
```

![|500](../../attach/Pasted%20image%2020230604172957.png)

算法流程:
```
cwnd = 1
loss_event = { 3ACK, TimeOut, None }
state = { SlowStart, CongestionAvoid }

Loop:
	send cwnd * TCP segments;
	If (state == SlowStart):
		For (each segmaent ACKed):
			cwnd <- cwnd + 1;
	Else If (state == CongestionAvoid):
		cwnd <- cwnd + 1;

	# state convert
	If (loss_event == 3ACK):
		ssthresh <- cwnd/2;
		cwnd <- ssthresh;
	Else If (loss_event == TimeOut):
		ssthresh <- cwnd/2;
		cwnd <- 1;
	Else If (cwnd > ssthresh):
		state <- CongestionAvoid
	Else:
		state <- state;
```

`MaxWnd = Min[rwnd, cwnd]`

## 连接

TCP 三次握手建立连接, 四次挥手释放连接

![|400](../../attach/Pasted%20image%2020230604174627.png)

![|400](../../attach/Pasted%20image%2020230604174640.png)

#### 使用TCP的协议

- SMTP
- FTP
- Telnet
- HTTP
- HTTPS