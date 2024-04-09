加强 Python 类型检查

```python
from Typing import Tuple, NewType

# 声明复杂类型:
def foo() -> Tuple[int, ...]:
	...

# 新类型, (辅助静态分析器类型检查)
NewType('Int32', int) # 混用时, 解释器会报错
Int32 = int           # 这样解释器会视为同一类型
```