`namedtuple` 创建轻量, 不可变的数据结构.

```python
import collecitons
Grade = collections.namedtuple('Grade', ('score', 'weight'))

# usage:
def report_grade(self, score, weight):
	self._grades.append( Grad(scare, weight) )
```