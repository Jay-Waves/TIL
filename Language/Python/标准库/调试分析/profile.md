---
url: https://docs.python.org/zh-cn/3.12/library/profile.html
date: 24-06-14
---

精确的性能分析需要**确定性**, 而 Python 解释器会引入大量开销. 但也由于解释器的存在, 无需在源代码中插入过多桩代码, 解释器自动为各个事件提供钩子 (hook), 精确分析所需要的开销相比解释器开销就很小了.

Python 标准库为 `profile` 接口提供了两种实现:
1. cProfile, 使用 C 实现, 运行开销小. 推荐使用.
2. profile, 纯 Python 实现, 会显著增加配置开销, 但更容易用 Python 扩展.

```bash
>>> import cProfile
>>> cProfile.run('re.compile("foo|bar")')

214 function calls (207 primitive calls) in 0.002 seconds

Ordered by: cumulative time

ncalls  tottime  percall  cumtime  percall filename:lineno(function)
     1    0.000    0.000    0.002    0.002 {built-in method builtins.exec}
     1    0.000    0.000    0.001    0.001 <string>:1(<module>)
     1    0.000    0.000    0.001    0.001 __init__.py:250(compile)
     1    0.000    0.000    0.001    0.001 __init__.py:289(_compile)
   3/1    0.000    0.000    0.000    0.000 _compiler.py:759(compile)
     1    0.000    0.000    0.000    0.000 _parser.py:937(parse)
     1    0.000    0.000    0.000    0.000 _compiler.py:598(_code)
     1    0.000    0.000    0.000    0.000 _parser.py:435(_parse_sub)
```

- `primitive calls` 指这些调用不是递归引起的.
- `ncalls` 被调用次数
- `tottime` 函数中消耗的总时间, 但不包括子函数时间
- `percall` totime/ncalls, 或 cumtime/ncalls
- `cumtime` 累积时间, 函数及其子函数消耗的总时间. 
- `3/1` 第一个指是调用总次数, 第二个是原始调用次数. 当函数不递归时, 两个值相同.

性能分析只有在被调用或执行的程序能确定返回时才可用, 如果解释器被终止, 则不会打印任何结果.

### 输出到文件

```python
import cProfile
cProfile.run('...', 'restats')

import pstats.Stats
# 分析 restats 文件中结果
```

### 命令行接口

直接分析另一个模块或脚本:

```bash
python -m cProfile [-o] [-s] (-m module | myscript.py)
```

### API

`profile.run(cmd, filename=None, sort=-1)`

```python
class profile.Profile:
	'''
		timer: 自定义计时器函数, 必须返回表示时间的数字.
		timeout: 每个时间单位持续时间的乘数, 如 timer 返回的值以千秒为单位, 
			timeout 应设置为 .001
	'''
	def __init__(timer=None, timeout=0.0, subcalls=True, builtins=True):
		...

	'''停止收集分析数据, 并在内部记录结果'''
	def create_stats(slf):
		...
		
	'''根据当前分析数据创建一个 Stats 对象, 并打印到 stdout'''
	def print_stats(slf, sort=-1):
		...

	'''将当前结果 (profile) 写入文件'''
	def dump_stats(slf, filename):
		...

	'''对命令进行性能分析, 使用 `exec()` 调用'''
	def run(slf, cmd):
		...

	'''性能分析, 附带指定的全局或局部环境'''
	def runctx(slf, cmd, globals, locals):
		...

import cProfile
with cProfile.Profile() as pr:
	...
```

