linux 有三种内存管理机制:
1. SLAB Allocator, 早期内存管理机制. 特点是有复杂缓存, 内存碎片少.
2. SLOB Allocator, (Simple List of Blocks), 更简洁.
3. SLUB Allocator, 最新默认内存管理机制, 取消了 slab 层, 缓存直接管理对象.

slab allocator


```
                        high

内核空间 

栈 vm_area( VM_READ | VM_WRITE )

文件映射与匿名映射 vm_area( VM_READ | VM_WRITE | VM_EXEC )

堆 vm_area( VM_READ | VM_WRITE | VM_EXEC )

BSS vm_area( VM_READ | VM_WRITE )

数据段 vm_area( VM_READ | VM_WRITE )

代码段 vm_area( VM_READ | VM_EXEC )

                        low
```