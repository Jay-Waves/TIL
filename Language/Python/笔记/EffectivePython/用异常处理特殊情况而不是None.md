对于特殊或非法的数据, 不使用None (因为会被误当作0), 而应该返回一个异常值 `exception`. 函数调用者应该处理这个异常, 并且这个异常应该被写在文档 (docs) 中

```python
def divide(a, b):
try:
	return a / b
except ZeroDivisionError as e:
	raise ValueError(‘Invalid inputs’) from e

x, y = 5, 2
try:
	result = divide(x, y)
except ValueError:
	print(‘Invalid inputs’)
else:
	print(‘Result is %.1f’ % result)
	
>>>
Result is 2.5
```
