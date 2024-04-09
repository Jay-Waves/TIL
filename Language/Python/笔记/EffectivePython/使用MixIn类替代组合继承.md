MIXIN 类用于单一功能扩展, 而不是实体父级, 不能实例化 (无 `__init__`).

```python
class ToDictMixin(object):
	def to_dict(self):
		return self._traverse_dict(self.__dict__)
		
	def _traverse_dict(self, instance_dict):
		output = {}
		for key, value in instance_dict.items():
			output[key] = self._traverse(key, value)
		return output
		
	def _traverse(self, key, value):
		if isinstance(value, ToDictMixin):
			return value.to_dict()
		elif isinstance(value, dict):
			return self._traverse_dict(value)
		elif isinstance(value, list):
			return [self._traverse(key, i) for i in value]
		elif hasattr(value, ‘__dict__’):
			return self._traverse_dict(value.__dict__)
		else:
			return value
```

使用 Mixin 类后, 可轻松将类对象转换为字典.
```python
class BinaryTree(ToDictMixin):
	def __init__(self, value, left=None, right=None):
		self.value = value
		self.left = left
		self.right = right

# test
tree = BinaryTree(10,	
		left=BinaryTree(7, right=BinaryTree(9)),
		right=BinaryTree(13, left=BinaryTree(11)))
print(tree.to_dict())
```


当二叉树对象维护一个指向父节点的引用, 环形引用会导致此前的 ToDictMixin 遍历死循环. 解决办法很简单, 使用类多态改变函数行为即可.
```python
class BinaryTreeWithParent(BinaryTree):
	def __init__(self, value, left=None, 
									right=None, parent=None):
		super().__init__(value, left=left, right=right)
		self.parent = parent
		
	def _traverse(self, key, value):
		if (isinstance(value, BinaryTreeWithParent) and key == 'parent':
			return value.value # Prevent cycles
		else:
			return super()._traverse(key, value)

# test
root = BinaryTreeWithParent(10)
root.left = BinaryTreeWithParent(7, parent=root)
root.left.right = BinaryTreeWithParent(9, parent=root.left)
print(root.to_dict())
```