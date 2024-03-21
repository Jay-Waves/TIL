研究统计 Go, channel-based 并发模型中的漏洞类型. 综述.

典型bug: 无缓冲 goroutine 会阻塞等待接收, 如果没有接收方, 就可能泄露. (除非 goroutine 自己设定一个 timer 来管理超时)

```go
func finishReq(timeout time.Duration) r ob {
	- ch := make(chan ob)
	+ ch := make(chan ob, 1)
	go func() {
		result := fn()
		ch <- result // block
	} ()
	select {
		case result = <- ch:
			return result
		case <- time.After(timeout):
			return nil
	 }
}
```

### Observations

1. Goroutines 执行时间更短, 但是使用更频繁 (比 C 中线程而言)
2. Go 中 shared memory thread communication 和 synchronization 仍被广泛使用, 但 Goers 也用很多 message-passing communication. 因此也导致并发漏洞的上下文更复杂.
3. 尽管 Go 初衷是用 message-passing 通信机制来简化并发模型和降低非直觉错误, 但是在调研的实际go应用中, blocking message-passing 比传统 shared-memory protection 造成了更多阻塞类型漏洞 (blocking bugs)

### Blocking Bugs

![](../../../attach/Pasted%20image%2020240318090441.png)

```go
var group sync.WaitGroup
group.Add(len(pm.plugins))
for _, p := range pm.plugins {
	go func(p *plugin) {
		defer group.Done()
	}
	- group.Wait()
}
+ group.Wait()
```

```go
// goroutine1
func goroutine1() {
	m.Lock()
-	ch <- request // blocks
+	select {
+		case ch <- request
+		default:
+	}
	m.Unlock()
}

// goroutine2
func goroutine2() {
	for {
		m.Lock() // blocks
		m.Unlock()
		request <- ch
	}
}
```

### Unblocking Bugs

Message Passing 比 shared memory 的方式产生非阻塞 bugs 的概率更小, 也确实是一种更安全的线程通信方式, 但是出现 bug 后更难查找修复.

匿名函数造成的数据竞争:
```go
for i:= 17; i<=21; i++{ // write
-	go func() { // create a new goroutine
+	go func(i int) {
		apiVersion := fmt.Sprintf("v1.%d", i) //read
		...
-	}()
+	}(i)
}
```

错误使用 WaitGroup:
```go
// func1
func (p *peer) send() {
	p.mu.Lock()
	defer p.mu.Unlock()
	switch p.status {
		case idle:
+			p.wg.Add(1)
			go func() {
-				p.wg.Add(1)
				...
				p.wg.Done()
			}()
		case stopped:
	}
}

// func2
func (p* peer) stop() {
	p.mu.Lock()
	p.status = stoppedp.mu.Unlock()
	p.mu.Unlock()
	p.wg.Wait()
}
```

错误传输消息占所有非阻塞错误的 20%, 这常常由错误使用 channel 导致的. 

```go
-select {
-	case <- c.closed:
-	default:
+		Once.Do(func() {
			close(c.closed)
+		})
-}
```

```go
	ticker := time.NewTicker()
	for {
+		select {
+		case <- stopCh:
+			return
+		default:
+		}
		f()
		select{
		case <- stopCh:
			return
		case <- ticker:
		}
	}
```

```go
-	timer := time.NewTimer(0)
+	var timeout <- chan time.Time
	if dur > 0{
-		timer = time.NewTimer(dur)
+		timeout = time.NewTimer(dur).C
	}
	select {
-		case <- timer.C:
+		case <- timeout:
		case <- ctx.Done():
			return nil 
	}
```