STL 是 C++ 标准库的一部分, 提供一系列模板化的数据结构和算法. 支持泛型, 高度性能优化.

主要部分为:
1. 容器, Containers
	- 顺序容器: 存储序列, 允许双向遍历. `vector`, `list`, `deque`, `array`, `forward_list`
	- 关联容器: 存储键值对. `map`, `multimap`, `set`, `multiset`
	- 散列容器: 即无序关联容器 `unordered_map`, `unordered_set`, `unordered_multimap`, `unordered_multiset`
	- 容器适配器: `queue`, `priority_queue`, `stack`, `bitset`
3. 算法, Alogirithms
4. 迭代器, Iterators
5. 函数对象, Funciton Objects
6. 适配器, Adapters, 将某种容器的接口适配成另一种容器.

pair, span, optional, variant, any

|          | 关联容器           | 散列容器                              | 顺序容器 |
| -------- | ------------------ | ------------------------------------- | -------- |
| 顺序     | 有序, 按键值排序   | 无序                                  | 有序, 按插入顺序         |
| 实现方式 | 红黑树             | 哈希表                                |          |
| 查找时间 | $O(\log n)$        | 平均 $O(1)$, 最坏 $O(n)$              |          |
| 插入时间 |                    |                                       |          |
| 删除时间 |                    |                                       |          |
| 空间     |  高                  | 高 (存储哈希表, 同时预留空间防止碰撞) |          |
| 场景     | 有序存储, 范围查询 | 查找和更新速度                        |          |
