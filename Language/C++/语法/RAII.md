RAII, Resource Acquisition is Initialization, 资源获取即初始化.

Resource 是指用来管理程序功能的对象. 如内存块, 文件句柄, 未释放的锁, 网络套接字, 数据库句柄等. **RAII 意味着, 对象的生命周期应在该变量的作用范围之内, 一旦 C++ 变量超出作用范围, 析构器 (destructor) 就会自动释放其资源.** 从这一点来说, RAII 更应该叫 "Scope-Bound Resource Management".

对于本地变量:
```cpp
{ // local resource 
	fp = ...;
} // call fp's destructor automatically
```

对于类成员: 使用 RAII 后, 资源显式声明, 在超出作用域时隐式释放.
```cpp
// without RAII
RawResourceHandle* handle=createNewResource();
handle->performInvalidOperation();  // exception thrown; or error exit 
...
// due to exception, never gets called so the resource leaks.
deleteResource(handle); 
```

使用 RAII 可以保证在程序不同结束路径下, 对象析构函数都会被调用:
```cpp
// with RAII
class ManagedResourceHandle {
public:
   ManagedResourceHandle(RawResourceHandle* rawHandle_) 
   : rawHandle(rawHandle_) {};
   ~ManagedResourceHandle() {delete rawHandle; }
   ... // omitted operator*, etc
private:
   RawResourceHandle* rawHandle;
};

ManagedResourceHandle handle(createNewResource()); // 获取即初始化
handle->performInvalidOperation(); // 作用域结束即释放
```

智能指针也是 RAII 原则的良好实践:
- `unique_ptr`.
- `shared_ptr`, 注意 `auto_ptr` 应被废弃.

使用 RAII 原则后, 对象的释放/获取直接和生命周期相关, 不必在逻辑中额外添加对应的释放操作, 对象生命周期结束后即**自动**调用析构函数. 此类对象的使用, 和直接使用 C 栈变量同样简洁, 但需注意析构函数不会自动释放堆成员 (如 new 或 malloc), 此时最好使用*智能指针*. 

在带 GC 语言中, 常见错误是不习惯手动释放底层对象, 导致资源泄露. python 用[上下文管理器](../../Python/标准库/contextlib.md)解决这个问题. 

> [What is meant by Resource Acquisition is Initialization (RAII)? Stack Overflow](https://stackoverflow.com/questions/2321511/what-is-meant-by-resource-acquisition-is-initialization-raii)
>
>  [Resource acquisition is initialization Wikipedia .](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)