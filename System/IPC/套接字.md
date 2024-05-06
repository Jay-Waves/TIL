Socket, 套接字提供点对点的双向进程间通信. 套接字仅能和同一域 (Comunication Domain) 中的套接字进行通信, 通信域定义在 `<sys/socket.h>`. Unix Domain 提供了套接字地址空间, 不同系统间通信被称为 Internet Domain, 在 Unix Domain 中的套接字被称为 Unix Paths. 

Internet Domain 使用 TCP/IP 网络协议进行通信. 套接字有如下几种类型:

| Socket Type       | 通信    | 面向连接 | 可靠传输                           | 协议           | 消息边界 | 类型名           | 长度限制                    |
| ----------------- | ------- | -------- | ---------------------------------- | -------------- | -------- | ---------------- | --------------------------- |
| Stream            | Two-way | Yes      | Sequenced, No loss                 | TCP            | No       | `SOCK_STREAM`    | No, stream-based            |
| Datagram          | Two-way | No       | No guarantee of order and delivery | UDP            | Yes      | `SOCK_DGRAM`     | Yes, limited by packet size |
| Sequential Packet | Two-way | Yes      | Sequenced, No loss                 | SCTP (uncomon) | Yes      | `SOCK_SEQPACKET` | Yes, packet-based           |
| Raw               |         |          |                                    | IP             |          |                  | Yes, limited by packet size |

## SOCK_SRTEAM

- server: `socket()` -> `bind(addr)` -> `listen()` -> `accept()` -> waiting for client
- client: `socket()` -> `connect(addr)` 

创建绑定套接字:
```c
// create a socket in `domain` and of `type`, and return a socket 
// handler (descriptor). if `protocol` is not specified, defaults to 
// protocol supports sepecified type.
int socket(int domain, int type, int protocol);

// bind a path (in Unix Domain) or a internet address (in Internet Domain)
// to a socket. There are different wayt to call `bind()`
int bind( int s, const struct sockaddr *name, int namelen);

// for Unix Domain, path <= 14 characters
#include <sys/socket.h>
bind (sd, (struct sockaddr *) &addr, length);

// for Unix Domain requires more characters
#include <sys/un.h>
bind (sd, (struct sockaddr_un *) &addr, length);

// fro Internet Domain
#include <netinet/in.h>
bind (sd, (struct sockaddr_in *) &addr, length);
```

连接套接字:
```c
// server
int listen(int s, int backlog)

// client, initiate a connection to server
int connect(int s, struct sockaddr *name, int namelen)

// for Unix Domain call
strcut sockaddr_un server;
connect (sd, (struct sockaddr_un *)&server, length);

// for Internet Domain call
struct sockaddr_in;
connect (sd, (struct sockadr_in *)&server, length);
```

数据传输:
```c
write()
read()
// send and recv have operational `flags`, formed from biwaise OR of 
// zero or more the following:
// `MSG_OOB`: out-of-band
// `MSG_DONTROUTE`: turn on `SO_DONTROUTE`, only for diagnostic
// `MSG_PEEK`: peek data present on socket, but not consumed, so a 
//  subsequent receive oepration will see the same data.
int send(int s, const char *msg, int len, int flags)
int recv(int s, char *buf, int len, int flags)
```

## SOCK_DGRAM

数据报套接字不需要建立连接. 

- server: `socket()` -> `bind(addr)` -> `recvfrom()`
- client: `socket()` -> `sendto()`

```c
// could use connect() and bind(), but accept() and listen() are not used
bind()
connect()

sendto()
sendmsg()
sendto()

recvfrom()
recvmsg()
recv()
```

## Socket Options

获取配置: getsockopt() 
