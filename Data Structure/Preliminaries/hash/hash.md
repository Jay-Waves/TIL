---
date: 2024-05-30
---

## 散列函数

基于散列的数据结构核心是**散列函数**, 即哈希函数. 满足:
- 输入键 (一般为字符串), 返回对应的值索引.
- 对于输入的键, 其索引值应尽量平均分布, 避免哈希碰撞导致的低效和错误.

下文中, 设 $hash()$ 为哈希函数, $key$ 为输入的键值, $index$ 指计算出的索引值, $n$ 指已发生碰撞的次数 (重复次数), $m$ 为索引值定义域大小 (表大小).

### 除留余数法

```c
typedef unsigned int uint;
uint hash_modulo(int key, uint m) {
    return key % table_size;
}
```

### 乘法哈希法

```c
uint hash_multipl(int key, uint m) {
	// A is a fraction close to (sqrt(5)-1)/2
    const double A = 0.6180339887; 
    double fractional_part = modf(key * A, &A); // <math.h>
    return (uint)(m * fractional_part);
}
```

### FNV哈希

Fowler-Noll-Vo Hash, 一种快速的非加密哈希函数, 广泛用于网络和数据存储.

```c
uint hash_fnv(const char *key) {
    uint hash = 2166136261u;
    const uint FNV_prime = 16777619u;
    
    while (*key) {
        hash ^= (unsigned char)(*key);
        hash *= FNV_prime;
        key++;
    }
    
    return hash;
}
```

### BKDR哈希

用于字符串哈希

```c
uint hash_bkdr(const char *key) {
    uint hash = 0;
    const uint seed = 131; // 31, 131, 1313, 13131, etc. are good seeds
    
    while (*key) {
        hash = hash * seed + (*key);
        key++;
    }
    
    return hash;
}
```

## 处理哈希碰撞

完美的哈希函数并不存在, 总会有不同的键值被映射到相同的索引值上, 称为**哈希碰撞**. 此时如果让两个不同的键使用同一内存区域, 就会导致错误. 索引值的定义域越小, 哈希碰撞的可能性越大, 所以散列型数据结构对内存要求较高.

当碰撞频率极高时, 散列表的效率可能劣化到 $O(n)$. 依赖数学链式关系的碰撞处理函数, *删除*操作较复杂, 因为不能直接删除.

### 两次哈希

两次哈希, double hashing.

```c
// n=0, if no collisions come
int index = hash1(key);
int step = hash2(key); // or use small changes on `key`

while ( in_use(hash_table[index]) ) {
	index = (index + step) % m;
}
```

### 独立链表

独立链表, chaining, 每个哈希表项皆为独立链表, 发生碰撞时, 向链表末尾添加新链表元.

```c
int index = hash(key);
struct hash_entry *entry = malloc(sizeof(struct hahs_entry));
hash_entry->next = hash_table[index]; // add entry to the head
hash_table[index] = new_entry;
```

### 其他方法

1. 线性探测 (linear probe): `i, i+1, i+2, ...`
3. 平方探测 (quadratic probe): `i, i+1, i+4, i+9, ...` 
4. 链式哈希 (linked hashing): `hash(k), hash(hash(k)), ...`