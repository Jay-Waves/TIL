Redis 的有序集合 (ZSET) 数据结构可以自动保持元素顺序, 并保持元素唯一性.
1. 按 score 排序
2. 若 score 相同, 按字典排序

```bash
> ZAdd racer_scores 8 "Sam" 10 "Royce"
2

> ZRange racer_scores 0 -1
1) "Sam"
2) "Royce"
> ZRevRange racer_score 0 -1
1) "Royce"
2) "Sam"
> ZRange racer_score 0 0 WithScores
1) "Sam"
2) "8"
# return elem whose score between [-infinity, 10]
> ZRangeByScore racer_socres -inf 10

# remove elemnt
> ZRem racer_socres "Sam"

# ZRevRank also provided
> ZRank racer_scores "Royce"
1

# simply recall ZAdd to update value, or:
> ZIncrBy reacer_scores 10 "Royce"
"20"
```

### Performance

Sorted Set 使用 Dual-Ported 数据结构, 包括一个 Skip List 和一个 Hash Table.
- 访问或插入是 `O(log(N))` 的
- 排序是 `O(1)` 的

### Commands

- `ZAdd key [NX|XX] [GT|LT] score member` 