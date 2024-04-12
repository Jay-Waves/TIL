Redis 列表是有序链接在一起的 String 集合. 用来:
- 实现栈, 队列结构
- 后台系统的排队管理

```bash
LPush bikes:repairs bike:1 bike:2 bike:3
RPush
# 获取多个元素, 如获取全部元素:
LRange 0 -1
LPop   # 从列表头弹出一个元素, 无元素将返回 nil
RPop   # 从列表尾弹出一个元素
LLen  
LMove  # 将元素从一列表移动到另一个
LTrim  # 裁剪列表
```

List 有特殊的阻塞命令, 如果列表为空指令将一直阻塞, 直到可执行或等待超时.

```bash
> RPush bikes:repairs bike:1 bike:2 
(integer) 2
# wait element for 1s, then return nil if not available
> BRPop bikes:repairs 1
1) "bikes:repairs"
2) "bike:2"
> BRPop bikes:repairs 1
1) "bikes:repairs"
2) "bike:1"
> BRPop bikes:repairs 1
(nil)
(2.01s)

# BRMove
```

**Redis 指令访问范围时, 使用闭区间.**

Redis 可以自动管理键:

```bash
# 当结构不存在时, Redis 会假设为任意类型, 直到其第一次被初始化.
> Del new_bikes
0
> LPush new_bikes bike:1 bike:2 bike:3 
3

> set new_bikes bike:1 
OK
> type new_bikes
String
> LPush new_bikes bike:2 bike:3 
(error) new_bikes 已经是一个字符串类型了 "bike:1"

# 为空后自动销毁 (Stream 类型不是)
> RPush bikes bike:1
1
> Exists bikes 
1 
> LPop bikes 
"bike:1" 
> Exists bikes
0

# 对不存在的键执行操作, 就像持有一个空结构.
> Del bikes
0
> LLen bikes
0
> LPop bikes
(nil)
```

### Performance

Redis 使用 linked list 来实现 List. 访问列表尾是极快的, 访问列表中部则应使用 [Sorted sets](Sorted%20sets.md) 数据类型.

访问头尾的操作: `O(1)`

操作数据, 或访问列表内部 (如 `LRange, LIndex, LInsert, LSet`): `O(n)`