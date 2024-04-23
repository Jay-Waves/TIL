> [尾调用优化 - 阮一峰的网络日志](https://www.ruanyifeng.com/blog/2015/04/tail-call.html)

**尾调用**指函数 `f()` 最后调用 `g()`, 此时 `f()` 不必等待 `g()` 返回才释放资源.

```go
func f(x){
	return g(x)
}
```

```go
// 不是尾调用, 但编译器可以优化为尾调用
func f(x) {
	y := g(x)
	return y
}

// 这也不是尾调用
func f(x){
	return g(x)+1
}
```

**尾调用优化**指函数 `f()` 尾调用 `g()` 后立即释放自己的调用栈帧 (call frame), 取代为 `g()` 的调用栈. 可以节省栈内存.

**尾递归**指函数 `f()` 尾调用 `f()` 自己. 使用尾调用优化, 可以避免递归导致的**栈溢出**错误. 实质是: 递归总能写成等价循环.

```go
// 不适用尾调用的递归
func factorial(n){
	if n==1 {return 1}
	return n*factorial(n-1)
}
factorial(5)

// 使用尾调用
func factorial(n, total){
	if n==1 {return total}
	return factorail(n-1, n*total)
}
factorial(5, 1)
```

使用尾调用的递归 `factorail` 要传入 `total`, 比较不自然, 可以改写:

```go
func tail_factorial(n, total){
	if n==1 {return total}
	return factorial(n-1, n*total)
}
func factorial(n){
	return tail_factorial(n,1)
}
```

使用函数式编程的**柯里化 (curring)**, 将多参数函数转换为单参数形式:
```python
def currying(fn, n):
	def inner(m):
		return fn(m,n) # 延迟调用
	return inner

def tail_factorial(n, total):
	if n== 1: return total
	return tail_factorial(n-1, n*total)

factorial = currying(tail_factorial, 1)

factorial(5)
```

采用默认值
```python
def factorial(n, total=1):
	if n==1:
		return total
	return factorial(n-1, n*total)
factorial(5)
```