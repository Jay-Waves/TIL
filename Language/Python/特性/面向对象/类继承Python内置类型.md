```python
class FrequencyList(list):
	def __init__(self, members):
		super().__init__(members)
	def frequency(self):
		counts = {}
		for item in self:
			counts.setdefault(item, 0)
			counts[item] += 1
		return counts
```

但如果不继承内置类型, 如何让类表现得像内置数据类型一样? 在类中手动模仿某些 Python 内置方法

```python
class BinaryNode(object):
	def __init__(self, value, left=None, right=None):
		self.value = value
		self.left = left
		self.right = right
		
class IndexableNode(BinaryNode):
	def _left_search(self, count, index):
		...
		return found, count

	def __getitem__(self, index):
		found, _ = self._left_search(0, index)
		if not found:
			raise IndexError('Index out of range')
		return found.value

tree = IndexableNode(
	10,
	left=IndexableNode(
		5,
		left=IndexableNode(2),
		right=IndexableNode(6)),
	right=IndexableNode(
		15, left=IndexableNode(11)))

tree[0] -> 2
list(tree) -> [2, 5, 6, 10, 11, 15]
# could not use len()
```

```python
 class SequenceNode(IndexableNode):
	 def __len__(self):
		 _, count = self._left_search(0, None)
		 return count

len(tree) -> 6
# still could not use count() or index()
```

由于手动实现这些内置方法可能不全, `collections.abc` 提供了一种模板, 仅需要 `__getitem__`, `__len__` 就可以自动实现其他内置方法, 而不需要手动实现所有内置类型的内置方法.

```python
from collections.abc import Sequence

class BadType(Sequence):
	pass

foo = BadType()
>>> TypeError: can't instantiate abstract class with abstract methods __getitem__, __len__, ...
```