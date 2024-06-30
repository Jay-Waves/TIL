Hash Table, 哈希表存储一系列无序"键值对", 将"键"输入哈希函数, 获取对"值"存储位置的索引, "键"总是唯一对应"值"的. 哈希表索引速度快 `O(1)`, 适合快速存取数据. [^1]

[^1]: https://github.com/jamesroutley/write-a-hash-table/ 代码实现详见 [hash_table.c](../../../src/hash_table.c). 

## 结构实现

下面给出 ASCII 字符串作为键和值的哈希表实现, 碰撞处理使用*两次哈希*.

```c
typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

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

## hash

哈希函数详见 [hash](hash.md)

```c
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

使用**两次哈希**来处理碰撞.

```c
static int ht_get_hash(
    const char* s, const num_buckets, int attempt
) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
```


## delete

`hash_table.delete(key)` 删除键值对, 键不存在时无事发生.

```c
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