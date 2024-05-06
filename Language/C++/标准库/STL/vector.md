## Vector

向量是一种容器.

|        | array          | vector     |
| ------ | -------------- | ---------- |
| 大小   | 不可变         | 可变       |
| 内存   | 栈上分配       | 堆上分配   |
| 接口   | 和原生数组类似 | 功能接口多 |
| 初始化 | 编译时         | 运行时           |

原型定义在 `#include <vector>`

**声明**: `vector<var_type> var;`

**方法**:
- `push_back()`
- `pop_back()` 
- `size_type` unsigned 类型, 可以保存向量长度 (甚至最大长度). 
- `size()` 返回向量长度, 类型为 `size_type`.

```cpp
// 用标准库定义的惯例变量, 可以保证不同环境下的一致性.
typedef vector::size_type vec_sz; 
vec_sz size=homework.size();
```

- `end()` 指向最后一个元素之后的位置
- `begin()` 指向第一个元素的位置

```cpp
for (const auto& num: numbers) {
	...
}
```
- `clear()` 
- `erase(start, end)`
- `at()` 等同于用 `[]` 计算下表
- `bool empty()` 检查是否为空