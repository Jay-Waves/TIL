异常处理:

```python
# can combine `try` with any of (`except`, `else`, `finally`)
try: 
	raise ExceptionXXX("log info")
	assert(C, "log inf") # say C as a logic condition
	...
except ErrorXXX:
	...
except AssertionError: # only catch exception from assert()
	...
except ErrorYYY: # catch some type of exceptions
	...
except Exception as e:# catch any exception inheriting from Exception, 
	# without SystemExit, KeyboardInterrupt, GneratorExit which inherit from BaseException
	...
else: # execute only if *no* exception caught
	...
finally: # always exelcute finally, not matter (normal exit, return , exception)
	...
```

上下文情景:
```python
with ... as ...:
	...
```

条件分支:

```python
if C : # say C a logic condition
	...
elif C :
	...
else:
	...
```

循环与迭代结构:

```python
while C:
	...
	break
	continue
else: # execute after normal exiting, without `break`
	...
```

```python
for xxx in iter_xxx:
	...
	yield xxx # new iterator
else:
	...
```