对序列中元素使用辅助函数进行累积操作.


```python
from functools import reduce

# reduce of sum
reduce((lambda x, y: x+y), list)

# reduct of multiply
reduce((lambda x, y: x*y), list)
```