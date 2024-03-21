ros2 系统混合了 channel-based 和 process/thread-based 并发模型的特点. 即各节点分别是独立进程, 进程间通过 ROS Channel 机制来通信; 各进程中有多个线程处理不同回调函数和功能, 通过共享内存来互相通信.

可能出现的 bug 有:
- 死锁 Deadlock
- 数据竞争 Data Race: write-write, write-read, read-dirty
- Buffer Overflow: 对通信缓存管理不善 (如map_size)
- Use-After-Free (UAF)
- Null-Pointer Dereference: like double free