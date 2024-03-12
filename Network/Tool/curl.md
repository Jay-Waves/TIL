测试联通性, 获取 URL 内容:
```bash
curl http://www.google.com
```

显示详细**链接配置**信息:
```bash
curl -v http://www.google.com
```

其他参数:
- `o`: 输出到其他文件 `curl -o myfile.txt`
- `H`: 添加 http 头 `curl -H "User-Agent: MyUserAgent" URL`
- `I`: 仅获取 http 头部 `curl -I URL`
- `u`: 用户认证 `curl -u username:password URL`
- `--limit-rate`: 限制速度
- `x`: 使用代理 `curl -x http://proxy-server:port http://www.example.com`
- `k`: 跳过 SSL 验证.
- `X`: 使用其他请求方法, 如 Post. `curl -X POST -d "user=yjw&passwd=123456" URL`
- `L`: 让 curl 跟随服务器重定向.
- `--resolve example.com:443:1.2.3.4` 手动指定域名的dns解析

### Invoke-WebRequest

Powershell 中 `curl` 实际是 `Invoke-WebRequest` 的别名 (不是原来的工具), 参数有变化:

- `-Uri` 
- `-Proxy` --> `-x`
- `-Headers` --> `-H`
- `-ContentType` --> `-X`

### 实例
	
windows 上 curl 默认不会走 clash proxy, 需要手动指定代理:
```bash
curl https://www.google.com -x http://127.0.0.1:7890
curl -v \
	--resolve github.com:443:140.82.112.4 https://github.com \
	-x http://127.0.0.1:7890
# 7890 是 clash 的端口
```

gfw 会污染 github.com 域名 (dns), 可修改 hosts 手动绕开, 不适用 dns:
1. 修改 hosts
2. `ipconfig /flushdns` 刷新 dns 缓存

> ch 对 github 并没有下死手, 只是用 dns 污染限速了. 
>  2023.10 更新, 现在好像下死手了.