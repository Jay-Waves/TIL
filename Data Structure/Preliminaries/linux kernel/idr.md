**maps, aka associative array, [hash table](../hash/hash%20table.md)**, 是存储键值对的结构, 主要有三个操作:
- `insert(key, value)`
- `remove(key)`
- `lookup(key)`

linux 内核中使用的 map 数据结构为 **idr**

## ID Allocator

ID Allocator, aka IDR 是内核中高效的哈希表结构, 将 UID 映射为指针.

```c
void idr_init(struct idr *idp);

/*
 * allocate a new *UID in [start, end)*, and associate it with `ptr`
 */
int idr_alloc(struct idr *idp, void *ptr, 
							int start, int end, fgp_t fgp_mask);

/*
 * allocates memory and disables preemption
 */
int idr_preload(gfp_t fgp_mask);

/*
 * reenables preemption
 */
int idr_preload_end(void);
```

### operator

```c
void *idr_find(struct idr *idp, int id);

void idr_remove(struct idr *idp, int id);

void idr_destroy(struct idr *idp);
```

### examples

```c
#include <linux/idr.h>
#include <linux/kernel.h>

void test(void) {
	struct idr map;
	const int n = sizeof("hello, world!");
	char *s = "hello, world!";
	int ids[n], i;

	idr_init(&map);
	for(i=0; *s; s++, i++) {
		ids[i] = idr_alloc(&map, s, 0/*start*/, n/*end*/, GFP_KERNEL);
	}
	while(i > 0) {
		i--;
		printk("%c\n", *(char*)idr_find(&map, ids[i]));
		idr_remove(&map, ids[i]);
	}
	idr_destroy(&map);
}
```