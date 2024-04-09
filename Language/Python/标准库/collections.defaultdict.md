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
