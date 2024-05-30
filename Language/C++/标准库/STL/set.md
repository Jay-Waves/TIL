## Set

Set 存储非重复元素, 需要提供 `Compare` 函数, 比较元素的键是否相同 `!comp(a, b) && !comp(b, a)`. Set 底层使用[红黑树](../../../../Data%20Structure/Preliminaries/tree/red-black%20tree.md)实现.

```cpp
#include <set>

template<
	class Key,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<Key>
> class set;
```