```python
def myfunc(arg1, arg2, ...):
	pass

for _ in range(5):
	args = (..., ...)
	thread = Thread(target=myfunc, args=args)
	thread.start()

for thread in threads:
	thread.join()
```


### Lock

尽管由于 Python [GIL](../特性/并发.md) 限制, 线程总是并发而不是并行, 但仍需要使用互斥锁 (Mutual-Exclusion Locks, mutex) 来保证其线程安全. 因为 GIL 并不保证对数据结构的操作都是原子的. 同样由于 GIL, Python 会平均每个线程的占用处理器时长.

```python
counter.cnt += 1

# 实际 Python 执行了:
var = getattr(counter, 'cnt')
var2 = var + 1
setattr(counter, 'cnt', var2)

# 未加锁的数据竞争:
varA = getattr(counter, 'cnt') # from A
varB = getattr(counter, 'cnt') # from B
varB2 = varB + 1               # from B
setattr(counter, 'cnt', varB2) # from B
varA2 = varA + 1               # from A
setattr(counter, 'cnt', varA2) # from A
```

借助[上下文管理](contextlib.md), 使用锁的方式很简单:

```python
class Counter:
	def __init__(self):
		self.lock = Lock()
		self.cnt = 0
	def plusone(self):
		with self.lock:
			self.cnt += 1
```

