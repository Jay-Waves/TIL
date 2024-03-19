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

### Unblocking Bugs

