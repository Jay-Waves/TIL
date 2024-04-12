[Redis](https://redis.io/docs/latest/develop/) 更像是一种数据结构服务器, 而不是关系型数据库; 更适合作为缓冲, 而不是存储海量数据. 实现语言是 C, 数据存储在内存中. 官方列举如下用途:
- database
- cache
- streaming engine
- message broker


### Redis DataStructure

- Strings: 表示字节序列
- Lists: 有序列表
- Sets: 无序无重复字符串集合, Redis 集合的插入/删除/访问操作为 $O(1)$ 时间.
- Hashes: 类字典的键值格式
- Sorted sets: 有序无重复字符串集合
- Streams: 类似仅追加日志, 记录时间顺序并交付处理.
- Geospatial indexes: 空间索引
- Bitmap: 可在字符串上使用比特操作
- Bitfields: 以字节序列组织的多个计数器

### Redis Cli 

connect to redis server :
```bash
redis-cli -h 127.0.0.1 -p 6379
```
