asyncio 库实现官方对*协程*的支持. 需要 python3.7 及以上.

```python
from asyncio import *

# 事件循环
run(my_coroutine) # 替代掉 get_event_loop

# 协程特性
async def my_coroutine():
	pass
await my_coroutine()

# 任务
create_task(my_coroutine) # 替代掉 ensure_future
gather(*coroutines, return_exceptions=False) # 运行多个协程, return_exceptions 为 True 时, 异常会作为结果返回而不是抛出 (类似 go 的 err)

# 阻塞调用
sleep(delay) # 类似 time.sleep, 但不会阻塞事件循环
to_thread(func, *args) # >py3.9, 用于执行阻塞IO操作

# 抢占式的同步原语
Lock()
Semaphore(value)
```