`heapq` 扩展了 `list`, 使其更接近堆 (优先队列) 的用法.

```python
from heapq import heappush, heappop, nsmallest

a = []
heappush(a, 5)
heappush(a, 3)
heappush(a, 1)

# always return smallest item
print(heappop(a), heappop(a))
>>> 1 3 

assert a[0] == nsmallest(1,a)[0] == 3

a.sort() # maintians heap invariant
```