Python 使用 `with` 结构来创建一个上下文情景, 避免了重复的 `try/finally` 结构:

```python
lock = Lock()
with lock:
	print('Lock is held')

# same as:
lock.acquire()
try:
	print('Lock is held')
finally:
	lock.release()
```

标准做法是定义有 `__enter__, __exit__` 方法的工具类. 但 contextlib 的 `contextmanager` 简化了工作:

```python
# 使用 context 临时改变日志级别

@contextmanager
def debug_logging(level, name):
	logger = logging.getLogger(name)
	old_level = logger.getEffectiveLevel()
	logger.setLevel(level)
	try:
		yield logger # with here as yield value.
	finally:
		logger.setLevel(old_level)

with log_level(logging.DEBUG, 'my-log') as logger:
	logger.debug("some msg!")
	logging.debug('this will not print')
```

context 常用来自动管理资源的释放:

```python
with open('/tmp/output', 'w') as handler:
	handler.write('some data')
```