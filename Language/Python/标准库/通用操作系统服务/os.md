

### 文件路径处理

Python 标准库的文件路径符号为 `/`, 在不同平台中 Python 解释器会自动解析为不同平台路径符号, 如 Windows 使用的 `\`. 

Python 将 `\` 视为转义字符, 如 `\n`. 如果必须使用, 可以进行二次转义 `\\`, 或使用[字符串字面量](../../语法/区分字节串与字符串字面量.md) `r'path\to\file'`. 

`os` 提供了 `os.path` 模块, 用于更标准的路径处理.

```python
import os
abspath = os.getcwd()
rootpath = os.path.abspath('..')
print(abspath)
print(rootpath)

ret = abspath.replace(rootpath, '.', 1)
```
