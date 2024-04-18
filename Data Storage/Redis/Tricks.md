`Watch` 监控 mykey 状态, 如果 `Wacth` 期间键被修改, 后续 `EXEC` 会直接退出.

```bash
# 使用 watch 实现 ZPop
WATCH zset
element = ZRANGE zset 0 0
MULTI
ZREM zset element
EXEC
```

Redis `Pub/Sub` 实现了一种发布/订阅通信机制. 该机制和 Redis 存储机制是完全隔离的.

```bash
> Subscribe channel1 channel2
> Publish channel1 "hello, boy!"
```