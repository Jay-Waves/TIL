- 同步锁 Mutex Lock
- 自旋锁 Spin Lock
- 读写锁 
- 条件变量: 阻塞线程, 直到另一个线程使其成立. (一般和互斥锁一起使用).
- 原子操作 Atomic
- 信号量 Semaphore
- 内存屏障 Memory Barrier
- 读复制更新 RCU

|            | 同步锁         | 自旋锁           |
| ---------- | -------------- | ---------------- |
| 获取方式   | 获取失败则休眠 | 获取失败则忙等待 |
| 获取速度   | 较慢           | 较快             |
| 临界区大小 | 无特殊限制     | 普遍较小         |
| 使用场景   | 无特殊限制     | 主要用于内核     |
| 休眠操作   | 可使用         | 不可使用                 |

非内核的锁其实都是伪锁, 因为操作系统时间片用完都会强行切走.

常见bug:
- 数据竞争 Data Race
- 原子性违反
- 顺序性违反 Order Violation
- 并发死锁 Concurrency Deadlock

## 检测方法

### 锁集分析 Lockset Analysis

自动识别某个变量是否应该被锁保护. 

> [Eraser: A Dynamic Data Race Detector for Multithreaded Programs -- Thomas Anderson](../../-%20docs/eraser.pdf)

发生序关系 happens-before relation

内存一致性模型

### 观察点检测 watchpoint