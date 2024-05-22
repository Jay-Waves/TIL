linux 有三种内存管理机制:
1. SLAB Allocator, 早期内存管理机制. 特点是有复杂缓存, 内存碎片少.
2. SLOB Allocator, (Simple List of Blocks), 更简洁.
3. SLUB Allocator, 最新默认内存管理机制, 取消了 slab 层, 缓存直接管理对象.

slab allocator
