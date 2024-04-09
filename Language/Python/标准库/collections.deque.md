双端队列, double-ended queue, 用于高效实现 FIFO 队列.

```python
from collections import deque

fifo = deque()
fifo.apend(1)      # Producer
x = fifo.popleft() # Consumer
```

普通 `list` 类型也是有序队列, 但是从列表头部删除/插入数据却是 $O(N)$ 复杂度的.