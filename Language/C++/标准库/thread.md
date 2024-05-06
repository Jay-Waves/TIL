标准库 `<thread>`

```cpp
#include<thread>

void work() {
}

std::thread t(work);
```

启动线程后, 要保证其正确汇入 (joined) 或分离 (detached).

```cpp
void main(){
	std::thread t(work);
	t.detach(); // 不等待线程结束, 分离执行
}
```

使用 `detach()` 时, 如果 `work` 访问了作用域 `main` 中的局部变量, 并且 `main` 结束释放后 `work` 尚未结束工作, 可能会造成未定义行为 (UAF).

```cpp
void main(){
	std::thread t(work);
	t.join(); // 等待线程结束, 收回所有资源
}
```