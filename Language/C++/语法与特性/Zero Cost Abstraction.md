> C++ obey the zero-verhead principle: You don't pay what you don't use. And further: you couldn't hand code any better what you do use. --Bjarne

**零开销抽象**意味着, 添加一个高级编程概念或语法时, 额外的开销会在编译期被消化, 不会带来额外的运行时开销, 即:
- no overhead in space
- no overhead in runtime

实际上运行时的零开销抽象很难(几乎不可能)实现, 参考 [Cppcon 2019](https://www.youtube.com/watch?v=rHIkrotSwcc)

### 有开销的抽象:

面向对象语言中的 *virtual methods* 概念, 常通过运行时查表的方式来确定实际的调用函数类型. 造成内存开销和运行速度降低.

高级语言中的 *garbage collection* 概念, 为了降低内存管理的难度, 使用 gc 自动监控和清理垃圾对象的资源.
