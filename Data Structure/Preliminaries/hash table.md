Hash Table, 哈希表存储一系列无序"键值对", 将"键"输入哈希函数, 获取对"值"存储位置的索引, "键"总是唯一对应"值"的. 哈希表索引速度快 `O(1)`, 适合快速存取数据, 也被称为 map, dictionary, symbol table. [^1]

[^1]: https://github.com/jamesroutley/write-a-hash-table/

## 哈希函数

哈希表的核心是**哈希函数**, 应满足:
- 输入键 (一般为字符串), 返回对应值的索引.
- 对于输入的键, 其索引值应平均分布, 以避免哈希碰撞导致的低效和错误. 

一个简单滴哈希函数:
```c
// hash_table.c
static int ht_hash(const char* s, int p, int m) {
	// p should be a prime larger that alphabet of ASCII (>128)
	// m is the number of buckets (k:v)
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(p, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}
```

完美哈希并不存在, 对于较小的域, 哈希碰撞发生较频繁, 此时不能让两个键使用同一内存区域. 

使用**两次哈希**来处理碰撞, 多个键形成了"链式关系", 实现简单. 缺点是*删除*操作复杂, "链式关系"上每个节点都不能完全删除, 否则链没办法索引.
```c
// double hashing to handle collisions:
// index = hash_a(string) + i * (hash_b(string)+1) % num_buckets, 
// where i is frequency of collisions, i=0 if there's no collision.
static int ht_get_hash(
    const char* s, const num_buckets, int attempt
) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
```

### 其他碰撞处理方法

1. 独立链表 (seperate chaining): 每个哈希表项皆为独立链表, 发生碰撞时, 向链表末尾添加新项.
2. 线性探测 (linear probe): `i, i+1, i+2, ...`
3. 平方探测 (quadratic probe): `i, i+1, i+4, i+9, ...` 
4. 二次哈希 (double hashing) 

后三种方法皆没有明确链遍历结束条件, 最差劣化为 `O(n)`.

## 结构实现

下面给出 ASCII 字符串作为键和值的哈希表实现, 碰撞处理使用*两次哈希*.

```c
// hash_table.h
typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static ht_item* ht_new_item(const char* k, const char* v);
ht_hash_table* ht_new();
static void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table* int);

// hash functions
static int ht_hash(const char* s, int p, int m);
static int ht_get_hash(const char* s, int num_buckets, int attempt)

// methods
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);
```



```c
// hash_table.c
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53; // fixed size
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); // default NULL
    return ht;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL)
            ht_del_item(item);
    }
    free(ht->items);
    free(ht);
}
```

## delete

`hash_table.delete(key)` 删除键值对, 键不存在时无事发生.

```c
// hash_table.c
static ht_item HT_DELETED_ITEM = {NULL, NULL};

void ht_delete(ht_hash_table* ht, const char* key) {
	// using lazy delete. 
	// with too many items deleted, ht will be slowed down significantly!
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM)
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--;
}

```

## Insert

`hash_table.insert(key, value)` 存储键值对, `key` 已存在时更新.

```c
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {
	    if (cur_item != &HT_DELETED_ITEM) // for deleted
		    if (strcmp(cur_item->key, key) == 0) { // for *update*
			    // meet the same key, update its value.
			    ht_del_item(cur_item);
			    ht->items[index] = item;
			    return 
		    }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    } 
    ht->items[index] = item;
    ht->count++;
}
```

## search

`hash_table.search(key)` 返回键对应的值, 如果键不存在返回 `NULL`

```c
char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
	    if (item != &HT_DELETED_ITEM) // for deleted
	        if (strcmp(item->key, key) == 0) 
	        // check if key is the same 
	            return item->value;
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    return NULL;
}
```