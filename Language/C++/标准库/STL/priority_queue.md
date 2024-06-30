---
path: <queue>
date: 2024-05-30
---

`std::priority_queue` 是 STL 中的一种[容器适配器](Container%20Adapters.md). 默认情况下是最[堆](../../../../Data%20Structure/tree/binary%20heap.md), 但是可自定义比较函数 `Compare()` 来改变行为. 

```cpp
template<
	class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

方法
- `emtpy()`
- `size()`
- `top()` 获取最高优先级元素
- `push()`
- `pop()`

模拟最小堆:

```cpp
using std::priority_queue;
using std::vector;
using std::greater;

priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
```