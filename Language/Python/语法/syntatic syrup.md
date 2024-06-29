---
date: 24-06-14
---

## 链式比较

Python 的逻辑操作不是二元的, 可以多元:

```python
a < x < b

# 等价于
a < x and x < b
```

## 单行代码

可以在单行写简单的 if, for. 虽然这不是最佳实践.

```python
if condition: do_something()

for dog in dogs: bark()
```

`if` 还可以这样写:

```python
do_something() if condition else None
```

`for` 可以使用列表推导式, 详见 [集合推导式与生成式](集合推导式与生成式.md)

```python
ids = [ dog.id for dog in dogs ]
```

## 字符串变量展开

py3 提供了一种替代 `format()` 方法的占位方式:
```python
print('%d %s' %(i, content))
#注意'引号才能变量替换
```

常见的format方式:
```python
'{:02x}, {:02X}'.format(hex_num1, hex_num2)
```

也可以:
```python
f'{a}, {b}'
```

## Ellipsis

省略号是一种特殊语法, 用于作为占位符省略具体实现.

```python
def myfunc():
	...

# 等价于
def myfunc():
	pass
```

类型提示时, 表示数组元素数量未知

```python
def func(arr: List[..., int]):
	...
```

定义抽象基类:

```python
from abc import ABC, abstractmethod
class MyABC(ABC):
	@abstractmethod
	def myfunc(slf):
		...
```