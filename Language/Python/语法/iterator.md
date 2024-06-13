## 推导式与生成式

### list comprehension

在列表中直接使用表达式
```python
a = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] 
squares = [x**2 for x in a] 
print(squares) 

>>> [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

**缺陷**: 每次执行都会创造一个新列表, 直接加载所有元素到内存中, 可能占大量内存 (要用它遍历, 就需要把所有这些新的列表都存起来, python往往不能释放掉这部分内存). 

> 具体见[[../内存管理/内存管理]]

应用:
- 创建等长空列表: `[0 for _ in list]`
- 奇数0偶数1: `[1 if i%2==0 else 0 for i in range(n)]

### generator expression

生成器提供了一种延迟计算的方式. 将数据定义为[枚举器](返回值使用迭代器而不是列表.md), 然后仅在实际请求时才(惰性)生成, 并不直接加载入内存.  使用方法就是用 `()` 替代推导式的 `[]`

e.g.
```python
it = (len(x) for x in open(‘/tmp/my_file.txt’))
#注意it此时是一个枚举类型, 不是常规数据类型
roots = ((x, x**0.5) for x in it)
#此处使用了嵌套(chain), 这样的速度也很快
print(next(roots))

>>> (15, 3.872983346207417) #output

# 生成式并不是生成了元组! 元组这样推导:
tuple(x**2 for x in range(5))
```

**缺陷**: 枚举器是消耗品, 只能使用一次. *但, 嵌套使用非常快*.

### enumerate

遍历时经常使用range. 有时想遍历sequence类型(list等), 又想要直到其序号, 就会使用`range(len(my_list))`这种表达形式, 但是使用enumerate()是更简洁的形式:

```python
for i, flavor in enumerate(flavor_list): 
	print(‘%d: %s’ % (i + 1, flavor))
```

其第二参数为"从哪里开始": `enumerate(my_list, begin_pos)`

## 返回值使用迭代器而不是列表

函数返回值想要列表 list 形式时, 应用 generator 替代.
这样的好处是: 
- 比处理列表更加简洁, 也更加高效
- generator 不会占用大量工作内存, 可以更好的应对大型数据

缺点是返回的迭代器只能使用一次, 见 [迭代器无法重用](勿重用迭代器.md). 可以用 `next()` 获取迭代器, 但更推荐 `list()` 直接生成整个列表

普通形式: 
```python
def index_words(text):
	result = []
	if text:
		result.append(0)
	for index, letter in enumerate(text):
		if letter == ' ':
			result.append(index + 1)
	return result
```

使用 generator 形式: 函数中将 iterator 递回给 `yield`
```python
def index_words_iter(text):
	if text:
		yield 0
	for index, letter in enumerate(text):
		if letter == ' ':
			yield index + 1
```

使用迭代器yield构造的函数, 运行机制有所不同. 它不是全部运行完后再调回, 当对其调用时, 并不立即执行而是保留在栈中, 直到外部单独调用iterator, 才开始一个一个的执行(`next()`). 

当然, 使用 `list()` 可以加速 iterator 的生成, 但超大数据时对内存的保护就消失了, 例如 `result = list(index_words_iter(address))`

## 迭代器不能重用

使用iterator缺点是只能使用一次, 当将iterator作为参数传递给函数时, 这一点会非常不方便. 因为无法保证该函数只使用一次该iterator, 因此会导致错误.
解决办法有三, 其一是将iterator优先转化为list使用; 其二是传递一个函数而非iterator, 该函数每次调用都会生成一个新的iterator; 其三是定义一个类, 该类模仿python的iterator protocol. 第三个方法最为自然.

### 方法一: 转化为list

```python
def normalize_copy(numbers):
	# 方法一: 在此处添加: number = list(numbers)
	total = sum(numbers)
	result = []
	for value in numbers:
		percent = 100 * value / total
		result.append(percent)
	return result

def read_visits(data_path):
	with open(data_path) as f:
		for line in f:
			yield int(line)

it = read_visits("/tmp/my_numbers.txt")
percentages = normalize_copy(it)
print(percentages)

>>>
[] # empty output? 
```

然而方法一却丢掉了使用iterator的好处: 方便处理大数据, 提高速度. 使用list()进行一步转换也非常不直观

### 方法二: 构造辅助函数

构造辅助函数funct, 将funct作为函数传递, 每次调用时都生成一个新的iterator. 
为了简洁, 辅助函数可以使用lambda函数(单行函数`lambda [list]: expr`)
```python
def normalize(new_iter):
	total  = sum(new_iter())
	result = []
	for value in new_iter(): 
		precent = 100 * value / total
		result.append(percent)
	return result

percentages = normalize(lambda: read_visits(data_path))
```

### 方法三: 构造类

> **python iterator protocol**:
> 指的是python如何执行for循环, 及其相关行为. 
> python执行`for x in foo`时, 实际上会调用`iter(foo)`, 然后`iter()`会调用`foo.__iter__`内置方法. `__iter__`方法需返回一个迭代器对象(iterator), 然后for循环会重复调用 `next()`内置函数来使用iterator, 直到其耗尽(StopIteration exception)

> 事实上, `iter()`接收到容器类型变量时(container type), 会依此生成一个新的iterator; 但如果它接收到一个iterator时, 它只会返回它自己(同一对象). 这导致了任何使用了iterator的迭代行为, 都会将其耗尽(只能使用一次)

```python
#事实上, 可以自己构造类, 对iterator protocol的行为进行模拟
class ReadVisits(object):
	def __init__(self, data_path):
		self.data_path = data_path
	def __iter__(self):
		with open(self.data_path) as f:
			for line in f:
				yield int(line)
```

```python
def normalize_defensive(numbers):
	if iter(numbers) is iter(numbers): # An iterator — bad!
		raise TypeError(‘Must supply a container’)
	total = sum(numbers)
	result = []
	for value in numbers:
		percent = 100 * value / total
		result.append(percent)
	return result

#可以发现, 用类模拟的container type效果和list类型一样
visits = [15, 35, 80]
normalize_defensive(visits) # No error
visits = ReadVisits(path)
normalize_defensive(visits) # No error

#传入迭代器, iter()只能返回其本身, 因此生成了异常.
it = iter(visits)
normalize_defensive(it)
>>>
TypeError: Must supply a container
```

