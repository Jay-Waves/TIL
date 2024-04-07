> Stefan Savage, Michael Burrows, Greg Nelson, Patrick Sobalvarro, and Thomas Anderson. 1997. Eraser: a dynamic data race detector for multithreaded programs. ACM Trans. Comput. Syst. 15, 4 (Nov. 1997), 391–411. https://doi.org/10.1145/265924.265927
> 
> File at [Eraser: A Dynamic Data Race Detector for Multithreaded Programs](../../../paper/Eraser,%20A%20Dynamic%20Data%20Race%20Detector%20for%20Multithreaded%20Programs.pdf)


- 检查是否存在免于数据竞争的充分条件
- 提出 Consistent Locking Discipline
	- 每个数据结构应被单个锁保护
	- 若有对数据结构的访问, 都在持有锁的情况下进行

检查变量被哪些锁保护:
```c
// x 可能被 A,B,AB 保护
AcquireLock(A);
AcquireLock(B);
x ++;
ReleaseLock(B);
ReleaseLock(A);

// x 可能被 B,C,BC 保护
AcquireLock(B);
AcquireLock(C);
x ++;
ReleaseLock(C);
ReleaseLock(B);

// 综上, x 应该被 B 保护
```

### 算法

Data Stuctures:
```
LocksHeld(t) = 线程t此时持有的锁集, 初始化为空
LockSet(x) = 潜在保护x的锁集, 初始化为全集
```

Algorithm:
```
if thread t acquires lock l:
	LocksHeld(t) = LocksHeld(t) union {l}
if thread t releases lock l:
	LocksHeld(t) = LocksHeld(t) - {l}
if thread t acceses x:
	LockSet(x) = LockSet(x) intersect LocksHeld(t)
	if LockSet(x) is empty:
		report "Data Race Warnings"
```

- Warnings do not imply a data race.
- No Warnings mean no data race in this execution.
- Misses some data races.

### 例外情况

误报: 本地初始化
```c
// T0
// Initialize a packet
pkt = new Pakcet();
pkt.Consumed = 0; <-

AcquireLock( SendQueueLock );
pkt = SendQueue.Top();
pkt.Consumed = 1;
ReleaseLock( SendQueueLock );
```

```c
// T1
// Process a pakcet
AcquireLock( SendQueueLock );
pkt = SendQueue.Top();
pkt.Consumed = 1;
ReleaseLock( SendQueueLock );
```

误报: 初始化后, 第一次写操作之前的所有读操作
```c
// T0
A = new A();
A.f = 0; <-

// publish A
globalA = A;
```

```c
// T1
f = globalA.f; <-
```

上述误报, 可以通过维护状态机来排除:

![|450](../../../attach/Pasted%20image%2020240407141722.png)

误报: 同一线程中, 被不同锁保护的位置.
```c
// remove from received packet
AcquireLock( RecvQueueLock );
pkt = RecvQueue.RemoveTop();
ReleaseLock( RecvQueueLock );

... //process pkt, which is thread local

// Insert into processed
AcquireLock( ProcQueueLock );
ProcQueue.Insert( pkt );
ReleaseLock( ProcQueueLock );

/* Algorithm would report warnings */
```

漏过: 
```c
// T0
// Initialize a packet
pkt = new Pakcet();
pkt.Consumed = 0; <-

AcquireLock( WrongLock );
pkt = SendQueue.Top();
pkt.Consumed = 1; <-
ReleaseLock( WrongLock );
```

```c
// T1
// Process a pakcet
AcquireLock( SendQueueLock );
pkt = SendQueue.Top();
pkt.Consumed = 1; <-
ReleaseLock( SendQueueLock );
```


