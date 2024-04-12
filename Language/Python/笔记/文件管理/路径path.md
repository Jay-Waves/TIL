```python
import os
abspath = os.getcwd()
rootpath = os.path.abspath('..')
print(abspath)
print(rootpath)

ret = abspath.replace(rootpath, '.', 1)
```

### python中路径使用`/`而不是windows中的`\`

#Trouble
`‘unicodeescape‘ codec can‘t decode bytes in position 2-3: truncated \\UXXXXXXXX escape`

- 解决办法:
1. 将`\`换位`/`
2. 使用`\`二次转义: 即将`\`转变为`\\`
3. 使用字符串前缀取消转义:`r'path\name'`, 见[[../EffectivePython/字符串前缀#python字符串前缀的知识|字符串前缀]]
