String 类型用于存储字节序列, 是 Redis 的基础类型, 包括:
- 文本
- 序列化对象
- 二进制数据

```bash
> set bike:1 Deimos
OK
> get bike:1
"Deimos"
```

redis 的键/值都是 String 类型. Redis 的数据结构体积都有限制, 如 String 的**体积不应大于 `512MB`**, 这也表明 Redis 的目的不是为了存储海量数据, 而是提供一种简单易用的数据存储方式.

```bash
# fail if key already exists
> set bike:1 bike nx
(nil)
# fail if key not existed
> set bike:1 bike xx
OK
```

```bash
# set multiple keys
> mset bike:1 "Deimos" bike:2 "ares" bike:3 "Vanth"
OK

# get multiple value, redis will return an *array*
> mget bike:1 bike:2 bike:3 
1) "Deimos"
2) "Ares"
3) "Vanth"
```

原子性加法 (atomic increment): redis 将 string 值解析为整数, 然后做加法. 类似命令如: `incr, incrby, decr, decrby`

```bash
> set total_crashes 0
OK
> incr total_crashes
(integer) 1
> incrby total_crashes 10
(integer) 11
```

### Performance

大部分操作是 `O(1)` 的.

`substr, getrange, setrange` 是 `O(n)` 的.