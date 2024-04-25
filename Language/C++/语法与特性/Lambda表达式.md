```cpp
[捕获列表] (参数列表) -> 返回类型 {
	函数体
}
```

- 捕获列表: 定义该Lambda表达式可以从封闭作用域捕获哪些变量.
- 参数列表: Lambda表达式接受的参数
- 返回类型

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // 打印每个元素
    std::for_each(numbers.begin(), numbers.end(), [](int number) {
        std::cout << number << std::endl;
    });

    // 计算所有元素的和
    int sum = 0;
    std::for_each(numbers.begin(), numbers.end(), [&sum](int number) {
        sum += number;
    });
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
```
