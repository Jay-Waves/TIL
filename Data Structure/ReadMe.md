## 指南

使用 kernel 风格 [kernel code style](../../Language/Coding%20Style/kernel%20code%20style.md)

大部分数据结构有三个核心接口:
- `search()`, 最基础的遍历, 用于一窥核心结构.
- `delete()`, 当不适用惰性删除时, 删除通常是最复杂的操作.
- `insert()`, 当键已存在时, 仅更新.

代码文件放在附录中, 笔记中只展示核心部分 (其实也挺完整).

## 名词表

- 序列: 指顺序容器, Sequence Containers, 元素按插入顺序存储, 通过索引访问.[^1] 
- 映射: 指关联容器, Assocaiative Containers. 一般基于树结构实现, 存储键值对.
- 散列: 指散列容器, Unordered Containers. 一般基于哈希表实现, 也存储键值对, 但存储位置无序, 由哈希函数决定, 查找较快.
- size, 指当前结构中有意义的数据个数.
- capacity, 指当前结构为数据开辟的总内存体积, 一般以字节为单位.
- peek, 指从当前数据结构中挑选出首个元素, 但不移除.
- pop, 指从当前数据结构中挑选出首个元素, 同时移除.
- push, 指向当前数据结构压入一个元素.
- xxx_init, 指某数据结构的初始化方法, 仅初始化数值, 不开辟内存.
- new_xxx, alloc_xxx, create_xxx, 指开辟内存 (声明对象) 同时对其初始化.

[^1]: 详见 [STL](../../Language/C++/标准库/STL/STL.md)

### 数据结构列表

- `stack` 栈
- `queue` 队列, `circular queue` [循环队列](../linked%20list/queue.md), `deque` [双端队列](../linked%20list/deque.md), [优先队列](../tree/binary%20heap.md)
- `linked list` 链表. `circular linked list` [循环链表](../linked%20list/circular%20linked%20list.md), `doubly linked list` [双向链表](../linked%20list/doubly%20linked%20list.md) 
- `list` [列表](../linked%20list/list.md), 变长有序集合. 在 linux kernel 语境下仍指链表.
- `Hash Table` [哈希表](../hash/hash%20table.md), 也被称为 `Map`, `Dictionary`, `Symbol Table`.


### 错误码

使用 C POSIC LIB `errno.h` 中定义的部分错误状态码. 详见 [appendix/errno.h](../../src/errno.h)
- `-ENOMEM` 内存不足, 无法分配内存. 即 `malloc` 失败时返回.
- `-EINVAL` 无效参数.
- `-ENOENT` 资源不存在, 如查找操作未找到指定键, 或数据结构已空.
- `-ENOSPC` 空间不足, 数据结构已满.
- `-EBUSY` 资源忙, 数据结构已被其他操作锁定.
- `-EEXIST` 资源已存在, 如插入操作中发现已有相同键.

这些错误码都是宏定义的负数整型, 但我们并不能保证某操作的正常返回值一定不是负数. 具体情境请具体分析, 如使用指针传参而不是返回值的形式.

## 参考

wiki

*Introduction to Algorithms*. Thomas H. Cormen.

*Data Structures and Algorithm Analysis in C*. Mark Allen Weiss.