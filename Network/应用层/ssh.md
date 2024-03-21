SSH, Secure Shell, 是一种远程登录会话安全协议. 开源软件 OpenSSH 是其实现, 服务端称为sshd, 客户端称为 ssh.

## 使用

### 初始化

`ssh-keygen -t rsa -C "email@xxx.com"`, `-t`代表类型, 有RSA和DSA两种. 

生成公钥, 默认存放于 `~/.ssh`, 有私钥 `~/.ssh/id_rsa`, 和公钥 `~/.ssh/id_rsa.pub`.  其中 `id_rsa` 权限是 600.

### 配置与登录

自定义配置, 在 `~/.ssh/config`, 文件结构如下:

```ssh
Host yjw_num1 # 定义主机别名
ServerAliveInterval 30
HostName 122.22.222.102 # 主机IP
Port 33033 # ssh 端口
User yjw
IdentityFile /home/yjw/.ssh/id_rsa # 私钥
ProxyCommand ssh ...
```

指定用户登录: `ssh yjw@192.168.0.103`

指定端口号登录: `ssh ssh 192.168.0.103 -p 2022`, 默认端口号是 22

### 配置多个SSH密钥

可能对多个网站, 需要不同ssh公钥. 比如github和gitlab使用不同的ssh. 默认只读取 `~/.ssh/id_rsa`, 

首先 `~/.ssh/id_rsa` 命名需要不同. 这个ssh publickey在gitlab等服务器网站设置中输入.

其次, 配置 `~/.ssh/config`:

```
# Host1
Host github.com
	HostName github.com
	User git
	PreferredAuthentications publickey
	IdentityFile ~/.ssh/id_rsa
Host gitlab.com
	HostName gitlab.com
	User git
	PreferredAuthentications publickey
	IdentityFile ~/.ssh/id_rsa2
	Port 5337 # ssh的端口号
```

Host 相当于一个别名, 替代为下面的 HostName. 这里有个坑, *如果路径包含特殊字符, 比如', 需要用双引号括起来.*

验证 `ssh -T git@github.com1`. 

### 配置服务器

启动/关闭服务:
```bash
systemctl start sshd (enable 保持启动)

systemctl stop sshd
```

## 其他配置

`-L`, `-D`

配置 `~/.ssh/config`: 防止特定网络环境下连接断开, 压缩数据, 多通道等
```
      TCPKeepAlive=yes
      ServerAliveInterval=15
      ServerAliveCountMax=6
      Compression=yes
      ControlMaster auto
      ControlPath /tmp/%r@%h:%p
      ControlPersist yes
```

ssh 有一个替代品 [`mosh`](https://mosh.org), 使用 UDP 协议, 连接更稳定并且带宽需求更少. 但是服务端也需要配置, 因为通常不内置.


## 原理

ssh 协议五阶段:
1. 建立 tcp 连接, 协商协议版本号
2. 认证: 口令 (Password) 认证或公钥认证, 若成功则继续
3. 用 Diffie-Hellman 算法生成会话密钥K, 会话ID
4. 客户端发起会话请求
5. 使用密钥K加密通信

公钥认证 (免密登录):
1. 客户端提前将公钥 $K_P$ 发送给服务器存储
2. 客户端发送 $\{User,\ K_P\}_K$, 服务器检查 $K_P$, 若存在继续
3. 服务器发送 $\{Challenge\}_{K_P}$, 客户端得到 $Challenge$
4. 客户端发送 $digest\{Challenge,\ K\}$, 服务器比对.

**SSH没有引入证书, 客户端通过检查 `~/.ssh/known_hosts` 中已知公钥, 来检查是否存在中间人**
