### OrderedDict

在 Python 3.10 之前的默认字典实现都是**无序**的. `OderedDict` 让字典内部保持键的插入顺序.

```python
from collections import OrderedDict

a = OrderedDict()
```

### namedtuple

`namedtuple` 创建轻量, 不可变的数据结构.

```python
import collecitons
Grade = collections.namedtuple('Grade', ('score', 'weight'))

# usage:
def report_grade(self, score, weight):
	self._grades.append( Grad(scare, weight) )
```

### defaultdict

默认值字典 (defaultdict) 在某键不存在时, 返回默认值. 需要提供一个函数来定义该默认值.


```python
# normal dict
stats = {}
key = 'my_counter'
if key not in stats:
	stats[key] = 0
stats[key] += 1
```

```python
from collections import defaultdict

# same as defaultdict(lambda: 0)
# because int is callable.
stats = defaultdict(int) 
stats['my_couonter'] += 1
```

和现有字典转化:
```python
existing_dict = {'a':1}
new_dfltdict = defaultdict(int, existing_dict)
new_dict = dict(new_dfltdict)
```

### deque

双端队列, double-ended queue, 用于高效实现 FIFO 队列.

```python
from collections import deque

fifo = deque()
fifo.apend(1)      # Producer
x = fifo.popleft() # Consumer
```

普通 `list` 类型也是有序队列, 但是从列表头部删除/插入数据却是 $O(N)$ 复杂度的.

## sortedcontainers

`sortedcontainers.SortedList` 基于红黑树实现的排序容器, 适合频繁插入的场景.
