## `__name__ == '__main__'`

Python 中 `__name__` 被设置为模块的名称, 当当前模块在**顶层代码环境**中执行时, `__name__` 被设置为 `__main__`. 

```python
>>> from concurrent.futures import process
>>> process.__name__
'concurrent.futures.process'
```

顶层代码环境指用户最先运行的那个 Python 模块, 从它开始导入所有需要的其他模块, 因此其也被称为 Python 的程序入口点. 惯例使用下面的代码段来封装 Python 程序的主要行为, `if __name__ == '__main__'` 将检查当前代码是否为顶层环境, 如果仅作为包被导入, 该代码段就不会执行, 方便了测试. 

`if __name__ == '__main__'` 段落位于全局, 所以应保持简洁, 应用另一个 `main()` 函数来封装程序主要行为. `main()` 返回整数值, 由 `sys.exit()` 获取, 这样模拟了在命令行接口程序的行为, 即返回值是一个表示执行状态的整数.

```python
import sys

def main():
	...
	return 0

if __name__ == '__main__':
	sys.exit(main())
```

## `__main__.py`

包使用 `__main__.py` 文件向外部提供一个命令行接口, 使用 `python -m hello` 时, 将执行 `__main__.py`. 该文件内部应保持简短, 不必添加 `if __name__ == '__main__'` 结构.

```
hello/
 ├── __init__.py
 ├── __main__.py
 └── hello.py
```