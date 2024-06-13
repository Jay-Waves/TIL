python 中, 使用 `subprocess` 管理多进程最自然.

```python
proc = subprocess.Popen(
	['echo', 'Hello form the child'],
	stdout = subprocess.PIPE
	)
out, err = proc.communicate()
print(out.decode('utf-8'))
```

等待子进程:

```python
proc =  subprocess.Popen(['sleep', '10'])
while proc.pool() is None:
	print('Working...')
	sleep 1
print('Exit status', proc.poll())
```

多子进程管理:

```python
def run_sleep(priod):
	proc = subprocess.Popen(['sleep', str(period)])
	return proc

procs = []
for _ in range(10):
	proc = run_sleep(1)
	procs.append(proc)
```

和子进程输入输出交互:

```python
def run_openssl(data):
	env = os.environ.copy()
	env['password'] = b'123345'
	proc = subprocess.Popen(
		['openssl', 'enc', '-des3', '-pass', 'env:password'],
		env=env,
		stdin=subprocess.PIPE,
		stdout=subprocess.PIPE
		)
	proc.stdin.write(data)
	proc.stdin.flush() # remove stdin cache, ensure openssl gets input.
	return proc
```

使用超时异常, 防止阻塞:

```python
proc = run_sleep(10)
try:
	proc.communicate(timeout=0.1)
except subprocess.TimeoueExpired:
	proc.terminate()
	proc.wait()

pritn('exit status: ', proc.poll())
```