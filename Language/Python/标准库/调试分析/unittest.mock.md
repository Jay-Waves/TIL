mock 用于在测试中模拟函数调用, 并使其返回预设的值.

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