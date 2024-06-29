---
url: https://docs.python.org/zh-cn/3.12/library/unittest.mock-examples.html
date: 24-06-14
---

在测试时, 可能被测代码所依赖的其他模块并没有写完, 此时可以用 `mock` 来模拟各种调用. 另一个优点是, `mock` 模拟的返回值能更高效地探索边界, 主动构造特殊值.

```python
from unittest.mock import MagicMock

class ProductionClass:
    def method(self):
        self.something(1, 2, 3)

    def something(self, a, b, c):
        pass

real = ProductionClass()
real.something = MagicMock()
real.method()
# 检查被调用时是否使用了正确的参数
real.something.assert_called_once_with(1, 2, 3) 
```

### 模拟方法

测试 `MyClass` 在传入某个对象 `something` 时, 该对象的 `close()` 方法是否被调用. 我们直接传入 `mock`, 它会在被调用时自动创建 `close()` 方法; 如果未被调用, 在测试中访问时将创建它, 并在 `assert_called_with()` 中引发一个失败异常.

```python
class MyClass:
	def closer(slf, something):
		something.close()

real = MyClass()
mock = Mock(name='foo')
real.closer(mock)
mock.close.assert_called_with()
```

### 模拟类

`patch('Foo')` 将对象 `Foo` 实例隔离为一个 mock, 我们可以用 `return_value` 对其进行模拟配置.

```python
def myfunc():
	instance = module.Foo()
	return instance.method()

with patch('module.Foo') as mock:
	instance = mock.return_value
	instance.method.return_value = 'the result'
	result = myfunc()
	assert result == 'the result'
```

### 追踪所有调用

`mock_calls` 属性记录了所有对 mock 的调用. 可用于判断预期的调用是否被执行, 或者是否按正确顺序来执行并且没有额外调用.

```bash
>>> mock = MagicMock()
>>> mock.method()
<MagicMock name='mock.method()' id='...'>

>>> mock.attribute.method(10, x=53)
<MagicMock name='mock.attribute.method()' id='...'>

>>> mock.mock_calls
[call.method(), call.attribute.method(10, x=53)]
```

### 模拟文件开关

模拟对象行为, 比如模拟文件的打开或关闭.

```python
from unittest.mock import mock_open, patch

m = mock_open(read_data='your_mock_data')
with patch('builtins.open', m):
    with open('any_path', 'r') as f:
        data = f.read()
print(data)  # 输出 'your_mock_data'
```

### 异步模拟

Python 3.8 支持对异步的模拟. 

#### 模拟异步上下文管理器

`AsyncMock` 和 `MagicMock` 支持使用 `__aenter__` 和 `__aexit__` 来模拟异步上下文管理器.

```python
class AsyncContextManager:
	async def __aenter__(self):
		return self
	async def __aexit__(self, exc_type, exc, tb):
		pass

mock_instance = MagicMock(AsyncContextManager())
async def main():
	async with mock_instance as result:
		pass

asyncio.run(main())
mock_instance.__aenter__.assert_awaited_once()
mock_instance.__asext__.assert_awaited_once()
```

#### 模拟异步迭代器

`AsyncMock` 和 `MagicMock` 支持用 `__aiter__` 来模拟异步迭代器.

```python
mock = MagicMock()  # or AsyncMock
mock.__aiter__.return_value = [1, 2, 3]

async def main():
	return [i async for i in mock]

asyncio.run(main())
```