函数返回值想要列表list形式时, 应用generator替代.
这样的好处是: 
- 比处理列表更加简洁, 也更加高效
- generator不会占用大量工作内存, 可以更好的应对大型数据
缺点是:
- 返回的generator只能使用一次, 其劣势集中体现在: [[使用迭代器作为参数时勿重用]]. 可以用next()获取iterator, 但更推荐list()直接生成整个列表

普通形式: 
```python
def index_words(text):
	result = []
	if text:
		result.append(0)
	for index, letter in enumerate(text):
		if letter == ‘ ‘:
			result.append(index + 1)
	return result
```

使用generator形式: 函数中将iterator递回给yield
```python
def index_words_iter(text):
	if text:
		yield 0
	for index, letter in enumerate(text):
		if letter == ‘ ‘:
			yield index + 1
```
> 使用迭代器yield构造的函数, 运行机制有所不同. 它不是全部运行完后再调回, 当对其调用时, 并不立即执行而是保留在栈中, 直到外部单独调用iterator, 才开始一个一个的执行(next()). 
> 当然, 使用list()可以加速iterator的生成, 但超大数据时对内存的保护就消失了, 例如`result = list(index_words_iter(address))`