协调多线程的方式之一是**流水线工作**, 即多线程组成*生成者-消费者管道*:

```
 worker1 -> worker2 -> worker3 -> ...
 `->` is queue with cache, like collections.deque
```

这种方式有几个棘手问题:
1. 如何监控管道是否有输入? 使用轮询结构, 此时后续线程持续饥饿, 占用 CPU 资源. 
```python
class Worker:
	def work(self):
		while True: # poll ++
			try:
				item = self.in_queue.get()
			except IndexError:
				sleep(0.01) # no work to do, tell cpu to come later
			else:
				result = self.func(item)
				self.out_queue.put(result)
				self.work_done += 1
``` 
2. 如何获取结果? 需要额外的结果队列 `done_queue`, 并轮询访问, 占用 CPU 资源.
3. 轮询如何退出? 捕获信号, 复杂.
4. 如何处理流水线阻塞? 线程处理任务有快有慢, 慢速线程的输入队列会不断堆积快速线程的输出, 直到耗尽系统资源. 需要限制缓存大小.

## Queue

鉴于手动实现生成-消费者队列的复杂性, 推荐使用 python 标准库的 `Queue`

```python
from queue import Queue
queue = Queue()

def consumer():
	queue.get() # blocking until something is put()

thread = Thread(target=consumer)
thread.start()
queue.put(object()) 
thread.join()
```

Queue 可以限制队列的缓存上限, 输入数量超出上限时, 所有 `put()` 将阻塞. 调用 `task_done()` 帮助 Queue 标记任务完成. 当所有任务被标记完成后, Queue 才可以被 `join()`, 不再阻塞.
```python
queue = Queue(1) # buffer size of 1

def consumer():
	work = queue.get()  # 2
	# ...
	queue.task_done()   # 3 mark `work` as completed

queue.put(object())   # 1
queue.join()          # 4
```

借助 `Queue` 完成管道访问, 并支持手动调用 `close()->join()` 停止工作:

```python
class ClosableQueue(Queue):
	SENTINEL = object() # also ok to use None
	def close(self):
		self.put(self.SENTINEL)
	def __iter__(self):
		while True:
			item = self.get()
			try:
				if item is self.SENTINEL:
					return # signal: no more input
				yield item
			finally: # context management
				self.task_done()

class Worker:
	def work(self):
		for item in self.in_queue: # like coroutine
			result = self.func(item)
			self.out_queue.put(result)

...
done_queue.close()
done_queue.join()
```

