存储键值对 (field-value), 值皆为 [String](Strings.md) 类型, 类似 Python 的字典.

```bash
> HSet bike:1 model Deimos brand Ergonom price 4972
> HGet bike:1 model
"DEimos"
> HGetAll bike:1

> HIncryBy bike:1 price 100
5072
```

### Performance

- 大部分为 O(1)
- `HKeys`, `HVals`, `HGetAll` 为 O(n)

### Commands

- `HDel`
- `HExists myhash field1` 寻找键是否存在.
- `HKeys` 返回所有键
- `HVals`
- `HLen` 
- `HMGet` 一次性获取多个键值
- `HMSet`
- `HRandField` 随机返回值
- `HScan` 迭代器
- `HSetNX` 设置值, 当且仅当该键不存在.
- `HStrLen` 返回某值的长度