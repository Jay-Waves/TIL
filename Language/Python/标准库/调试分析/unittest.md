## 组织测试

```python
import unittest

class Test1(unittest.TestCase):
    
    def test_addition(self): # 以 test_ 开始
        self.assertEqual(1 + 1, 2)

    def test_subtraction(self):
        self.assertEqual(3 - 1, 2)

class Test2(unittest.TestCase):
	@classmethod
	def setUpClass(cls):
		pass
		
	def setUp(self):
		self.list = [1, 2, 3]

	def test_addition(self):
		self.assertEqual(self.list[0] + self.list[1], self.list[2])

	def tearDown(self):
		del self.list


# 自定义测试套件
def suite():
	suite = unittest.TestSuite()
	suite.addTest(Test1('test_subtraction'))
	suite.addTest(Test2('test_addition'))
	...
	return suite


if __name__ == '__main__':
	runner = unittest.TextTestRunner()
	runner.run(suite())
	# 正常情况下, main() 就可以自动寻找并执行该模块下的所有测试用例.
	# unittest.main()
```

工作目录就像:
```
+
|
+ - test_mypkg.py
+ - mypkg.py
+ - __init__.py
```

使用命令行运行单元测试, 这里有[很多方法](https://docs.python.org/zh-cn/3.12/library/unittest.html#command-line-interface)

```bash
python -m unittest test_mypkg.py
```

详细测试流程:
1. 依次执行测试套件 `TestSuite` 中的测试类 `TestCase`
2. 对于测试类, 执行 `setUpClass()`, 设置公共类属性.
3. 对于每个类实例, 执行 `setUp()` , 初始化测试实例所需的资源.
4. 对于每个类实例, 执行 `test_...()` 定义的测试方法.
5. 对于每个测试方法, `self.assertXXXX()` 断言全部通过时, 测试方法才通过.
6. 对于每个类实例, 全部测试方法执行完后, 执行 `tearDown()` 清理实例资源.

### 跳过测试

当某项测试预计会失败, 或者在特定测试条件下无需测试时, 可以直接跳过. unittest 主要通过函数装饰器来实现测试跳过.

```python
class Test(unittest.TestCase):
	@unittest.skip("just skip")
	def test_nothing(self):
		self.fail("branch not reached")

	@unittest.skipIf(__lib_version__ < (1, 3), "low version")
	def test_func(self):
		pass

	@unittest.expectedFailure
	def test_fail(self):
		self.fail("branch not reached")
```

当然, `skipIf()` 直接作为分支写到函数更自然, 方法内可以用 `self.skipTets("")` 来跳过当前测试. `skip()` 则主要用于自定义装饰器:

```python
def skipUnlessAttr(obj, attr):
	if hasattr(obj, attr):
		return lambda func: func
	return unittest.skip()
```

### 断言方法

| 方法                     | 检查                       | 支持版本 |
| ------------------------ | -------------------------- | -------- |
| `assertEqual(a, b, msg=None)`[^1]      | `a == b`, 支持多种类型                   |          |
| `assertNotEqual(a, b)`   |                            |          |
| `assertTrue(x)`          | `bool(x) is True`          |          |
| `assertFalse(x)`         | `bool(x) is False`         |          |
| `assertIs(a, b)`         | `a is b`, `id(a) == id(b)` | >= 3.1    |
| `assertIsNot(a, b)`      |                            |          |
| `assertIsNone(x)`        |                            |          |
| `assertIsNotNone(x)`     |                            |          |
| `assertIn(a, b)`         | `a in b`                   |          |
| `assertNotIn(a, b)`      |                            |          |
| `assertIsInstance(a, b)` | `isinstance(a, b)`         |  >= 3.2        |
|  `assertIsNotInstance(a, b)`                        |                            |          |

- `asserEqual(a, b)`
- `assertTrue(x)`
- `assertFalse(x)`
- `assertRaises(exception, callable, *args, **kwargs)`

[^1]: 注, 几乎所有断言都支持指定错误信息 msg

检查异常, 警告或[日志](../通用操作系统服务/logging.md):

| `assertRaises(exception, func, *args, **kwds)` | `func(*args, **kwds)` 引发了异常 `exception` |        |
| ---------------------------------------------- | -------------------------------------------- | ------ |
| `assertRaisesRegex(exc, r, func, ...)`         | 异常消息和正则表达式 `r` 匹配                | >= 3.1 |
| `assertWarns(warn, func, ...)`                 | `func(...)` 引发了警告                       | >= 3.2 |
| `assertLogs(logger, level)`                    | `logger` 使用了至少 `level` 级别来写入日志                                             |        |

更细致的检查:

|                              |                      |        |
| ---------------------------- | -------------------- | ------ |
| `asserAlmostEqual(a, b, places=7)`     | `round(a-b, 7) == 0` |        |
| `assertNotAlmostEqual(a, b)` |                      |        |
| `assertGreater(a, b)`        | `a > b`              | >= 3.1 |
| `assertGraterEqual(a, b)`    | `a >= b`             |        |
| `assertLess(a, b)`           | `a < b`              |        |
| `assertLessEqual(a, b)`      |                      |        |
| `assertRegex(s, r)`          | `r.search(s)`        | >= 3.2       |
