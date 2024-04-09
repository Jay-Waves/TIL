在 Python 3.10 之前的默认字典实现都是**无序**的. `OderedDict` 让字典内部保持键的插入顺序.

```python
from collections import OrdereDict

a = OrderedDict()
```