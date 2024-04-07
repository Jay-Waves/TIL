### Test Class

method should begin with `test_`

```python
import unittest

class SimpleTest(unittest.TestCase):
    
    def test_addition(self):
        self.assertEqual(1 + 1, 2)

    def test_subtraction(self):
        self.assertEqual(3 - 1, 2)
```

setup resources, or clean up after test:
```python
class Test(unittest.TestCase):
	@calssmethod
	def setUpClass(cls):
		# 该方法在最开始运行一次, 设置测试用例的公共属性, 如数据等.
		pass
		
	def setUp(self):
		# 该方法在每个测试方法前运行, 初始化每个测试都要用的数据, 比如一个类实例.
		self.list = [1, 2, 3]

	def tearDown(self):
		del self.list
```

### Assertions

- `asserEqual(a, b)`
- `assertTrue(x)`
- `assertFalse(x)`
- `assertRaises(exception, callable, *args, **kwargs)`

### Run test

```python
if __name__ == '__main__':
	unittest.main()
```

run test suites:

```python
def suite():
    suite = unittest.TestSuite()
    suite.addTest(SimpleTest('test_addition'))
    suite.addTest(AdvancedTest('test_length'))
    return suite

if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())
```

### unittest.mock

模拟对象行为, 比如模拟文件的打开或关闭.

```python
from unittest.mock import mock_open, patch

m = mock_open(read_data='your_mock_data')
with patch('builtins.open', m):
    with open('any_path', 'r') as f:
        data = f.read()
print(data)  # 输出 'your_mock_data'
```