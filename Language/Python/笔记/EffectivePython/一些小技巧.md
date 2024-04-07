### 函数参数返回多个数值
1. 返回多个数值其实是通过元组实现的, 而元组可以省略括号, 所以通常看来是这样:
```python
def func(a, b):
	return a, b, c
```
2. 接受函数返回的多个数值, 用的方法有两种. 其一是利用弃用命名字符`_`, 另一种是使用\*通配符
```python
_, _, a = func(a,b)
# or
*b, a = func(a,b)
```

### swap()实现:
利用python元组的解包性质, 可以便捷实现swap功能:
`a, b = b, a`

*实际上, 这可以理解为利用了一次元组'不可变'性质. b, a其实是(b, a)缩写, RHS先被赋值给一个元组, 再由元组进行赋值, 由于元组是不可变的, 所以b, a不需要重新开辟新地址*

### 闭包:

当你在一个函数中定义另一个函数，并在内部函数中引用外部函数的局部变量，这个内部函数就可以作为闭包来“捕获”并保留对这些局部变量的访问权。但是，如果你只是在一个函数（作为闭包）中调用另一个外部的函数，那么这个外部函数默认情况下是不能直接访问闭包的外部变量的，除非你明确地将这些变量作为参数传递给它。

为了使这一点更为清晰，考虑以下简化的例子：

```python
def outer(x):     
	def inner():         
		return x  
		# inner函数可以直接访问外部函数的局部变量x     
	return inner  
closure = outer(10) 
print(closure())  # 输出：10  

def another_function(y):     
	return y  
	
def outer_2(x):     
	return another_function(x)  
	# 虽然在outer_2中调用，但another_function不能直接访问x，除非作为参数传递  
print(outer_2(20))  # 输出：20
```


在第一个例子中，`inner`是一个闭包，它可以直接访问外部函数`outer`的局部变量`x`。

在第二个例子中，`outer_2`调用`another_function`并传递`x`给它。尽管`another_function`在`outer_2`的上下文中被调用，但它不能直接访问`outer_2`的局部变量`x`，除非`x`被明确地作为参数传递给它。