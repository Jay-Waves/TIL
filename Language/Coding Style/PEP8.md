## PEP8

PEP8 (Python Enhancement Proposal 8) 规定了一系列约定和最佳实践, 广泛用于 Python 标准库和第三方库. 主要内容有:

1. 缩进为 4 空格, 不要使用 `tab`. (Linus 狂怒)
2. 行长度不超过 79 字符, 尤其是注释.
3. 函数和类之间使用两个空行, 类中方法间使用一个空行.
4. 导入分为三组: 标准库, 第三方库和本地库. 每组间用空行分隔.
5. 函数的参数列表, 索引或切片紧跟的括号中, 不要加空格.
6. 命名规则, [见下文](PEP8.md#命名规则).
7. 注释单行使用 `#`, 多行使用 `"""`.
8. 谨慎使用缩写, 仅使用公认缩写, 如 `msg, flg, tmp, inc`.
9. 变量类型不要加入变量名中, python 是灵活的动态类型.

> PEP8 官方文档: https://peps.python.org/pep-0008/

## 命名规则

蟒蛇命名法 (SnakeCase) 适用于大多数情况:
* 模块化:
    - public: module_name
    - internal: _module_name
* 包名:
    - public: package_name
* 方法名:
    - public: method_name()
    - internal: _method_name() (被保护的)
* 函数名:
    - public: function_name()
    - internal: _function_name()
* 全局变量名/类的变量名:
    - public: global_var_name
    - internal: _global_var_name
* 实例对象名:
    - public: instance_var_name
    - internal: _instance_var_name (被保护的)

少数使用大驼峰法 (CapWords):
* 类名:
    - public: ClassName
    - internal: _ClassName
* 报错名:
    - public: ExceptionName

极少数全部使用大写: 全局常量名/类的常量名
- public: GLOBAL_CONSTANT_NAME
- internal: _GLOBAL_CONSTANT_NAME   

***

### Docstrings

给每个*函数, 模块, 类*写文档. 注意, python ide 大多支持注释使用 markdown.

```python
def main():
	"""
	function that prints out 'hello, world!'

	Args: 
	- ...
	- ...
	Returns:
		...
	"""
	print("hello, world!")

# python 提供了内置方法来获取文档
print(repr(main.__doc__))
```

利用内建支持, python 社区有很棒的文档生成工具:
- Sphinx
- Read the Docs
- 文档标准: [PEP-257](http://www.python.org/dev/peps/pep-0257/)

每个文件模块应该有顶层注释, 由 `"""` 包裹.

```python
#!/usr/bin/python3
"""
Library for testing words for various linguistic patterns

More details about this modules.....

Available functions:
- func1: ....
- func2: ....

Also a good place to introduce command-line interaction
"""
```

每个类应该有注释, 和模块注释类似.

```python
class MyClass:
	"""
	This is my class

	Used for ......

	Public attributes and methods:
	- a: ...
	- b: ...
	"""
```