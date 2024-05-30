- **std::stack**：
    
    - **特点**：基于LIFO（后进先出）原则的栈适配器，默认底层容器为 `std::deque`。
    - **接口**：`push`、`pop`、`top`、`empty`、`size`。
    - **适用场景**：需要LIFO特性的场景。
- **std::queue**：
    
    - **特点**：基于FIFO（先进先出）原则的队列适配器，默认底层容器为 `std::deque`。
    - **接口**：`push`、`pop`、`front`、`back`、`empty`、`size`。
    - **适用场景**：需要FIFO特性的场景。
- **std::priority_queue**：
    
    - **特点**：基于优先级的队列适配器，底层容器通常为 `std::vector` 并使用堆排序。
    - **接口**：`push`、`pop`、`top`、`empty`、`size`。
    - **适用场景**：需要优先级调度的场景。

```cpp
#include <iostream>
#include <stack>

int main() {
    std::stack<int> stack;

    // 入栈
    stack.push(1);
    stack.push(2);
    stack.push(3);

    // 访问和出栈
    while (!stack.empty()) {
        std::cout << "Top: " << stack.top() << std::endl;
        stack.pop();
    }

    return 0;
}

```