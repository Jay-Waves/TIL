```python
import os
abspath = os.getcwd()
rootpath = os.path.abspath('..')
print(abspath)
print(rootpath)

ret = abspath.replace(rootpath, '.', 1)
```

