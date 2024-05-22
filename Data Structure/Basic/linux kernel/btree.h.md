---
author: Joern Engel, Peter Zijlstra
license: GPL2
path: include/linux/btree.h
---

## Simple In-memory B+Tree

### Using Example

```c
// using examples
head->mempool = mempool_create(0, btree_alloc, btree_free, NULL);
if (!head->mempool)
	return ENOMEM;
return 0;

// then, destroy it manually:
mempool_free(head->node, head->mempool);
mempool_destroy(head->mempool);
head->mempool = NULL;

```

### Header 

通过排序 `unsigned long` 类型键值, B+树用于将 `unsigned long` 键值映射为数据(指针), 此处使用 `long` 存储指针. B+树包含一个头结构, 记录数据信息以及子节点. 内部节点包含多个键和指向子节点的指针, 叶子节点还包含指向数据的指针.

```c
#ifndef BTREE_H
#define BTREE_H

/**
 *
 * Each node in this implementation has the following layout:
 * [key1, key2, ..., keyN] [val1, val2, ..., valN]
 */

struct btree_head {
	unsigned long *node;     // first node in tree
	mempool_t     *mempool;  // mempool used for node allocations
	int            height;   
};

/* btree geometry */
struct btree_geo;

/**
 * btree_init_mempool - initialise a btree
 */
void btree_init(struct btree_head *head);

/**
 * Look up a key in the btree. 
 * This function returns the value for the given key, or %NULL.
 */
void *btree_lookup(struct btree_head *head, struct btree_geo *geo,
		   unsigned long *key);

/**
 * Insert an entry into the btree
 *
 * @head: the btree to add to
 * @geo: btree geometry
 * @key: the key to add (must not already be present)
 * @val: the value to add (must not be %NULL)
 *
 * This function returns 0 if the item could be added, or an
 * error code if it failed (may fail due to memory pressure).
 */
int __must_check btree_insert(struct btree_head *head, 
					struct btree_geo *geo,
					unsigned long *key, 
					void *val, gfp_t gfp);
/**
 * btree_update - update an entry in the btree
 *
 * @head: the btree to update
 * @key: the key to update
 * @val: the value to change it to (must not be %NULL)
 *
 * This function returns 0 if the update was successful, or
 * -%ENOENT if the key could not be found.
 */
int btree_update(struct btree_head *head, struct btree_geo *geo,
		 unsigned long *key, void *val);
/**
 * btree_remove - remove an entry from the btree. 
 *
 * @head: the btree to update
 * @key: the key to remove
 *
 * This function returns the removed entry, or %NULL if the key
 * could not be found.
 */
void *btree_remove(struct btree_head *head, struct btree_geo *geo,
		   unsigned long *key);

/**
 * btree_merge - merge two btrees
 *
 * @target: the tree that gets all the entries
 * @victim: the tree that gets merged into @target
 *
 * The two trees @target and @victim may not contain the same keys,
 * that is a bug and triggers a BUG(). This function returns zero
 * if the trees were merged successfully, and may return a failure
 * when memory allocation fails, in which case both trees might have
 * been partially merged, i.e. some entries have been moved from
 * @victim to @target.
 */
int btree_merge(struct btree_head *target, struct btree_head *victim,
		struct btree_geo *geo, gfp_t gfp);

/**
 * btree_last - get last entry in btree
 *
 * @head: btree head
 * @key: last key
 *
 * Returns the last entry in the btree, and sets @key to the key
 * of that entry; returns NULL if the tree is empty, in that case
 * key is not changed.
 */
void *btree_last(struct btree_head *head, struct btree_geo *geo,
		 unsigned long *key);

/**
 * btree_get_prev - get previous entry
 *
 * @head: btree head
 * @key: pointer to key
 *
 * The function returns the next item right before the value pointed to by
 * @key, and updates @key with its key, or returns %NULL when there is no
 * entry with a key smaller than the given key.
 */
void *btree_get_prev(struct btree_head *head, struct btree_geo *geo,
		     unsigned long *key);


/* internal use, use btree_visitor{l,32,64,128} */
size_t btree_visitor(struct btree_head *head, struct btree_geo *geo,
		     unsigned long opaque,
		     void (*func)(void *elem, unsigned long opaque,
				  unsigned long *key, size_t index,
				  void *func2),
		     void *func2);

/* internal use, use btree_grim_visitor{l,32,64,128} */
size_t btree_grim_visitor(struct btree_head *head, struct btree_geo *geo,
			  unsigned long opaque,
			  void (*func)(void *elem, unsigned long opaque,
				       unsigned long *key,
				       size_t index, void *func2),
			  void *func2);


extern struct btree_geo btree_geo32;
#define BTREE_TYPE_SUFFIX l
#define BTREE_TYPE_BITS BITS_PER_LONG
#define BTREE_TYPE_GEO &btree_geo32
#define BTREE_KEYTYPE unsigned long
#include <linux/btree-type.h>

#define btree_for_each_safel(head, key, val)	\
	for (val = btree_lastl(head, &key);	\
	     val;				\
	     val = btree_get_prevl(head, &key))

#endif /* BTREE_H */
```

### Toolkit

```c
// static tools in lib/btree.c

// for array of type `long`
static int longcmp(const unsigned long *l1, const unsigned long *l2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (l1[i] < l2[i])
			return -1;
		if (l1[i] > l2[i])
			return 1;
	}
	return 0;
}

static unsigned long *longcpy(unsigned long *dest, const unsigned long *src,
		size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];
	return dest;
}

static unsigned long *longset(unsigned long *s, unsigned long c, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		s[i] = c;
	return s;
}

// for btree geomotry
static void dec_key(struct btree_geo *geo, unsigned long *key)
{
	unsigned long val;
	int i;

	for (i = geo->keylen - 1; i >= 0; i--) {
		val = key[i];
		key[i] = val - 1;
		if (val)
			break;
	}
}

static unsigned long *bkey(struct btree_geo *geo, unsigned long *node, int n)
{
	return &node[n * geo->keylen];
}

static void *bval(struct btree_geo *geo, unsigned long *node, int n)
{
	return (void *)node[geo->no_longs + n];
}

static void setkey(struct btree_geo *geo, unsigned long *node, int n,
		   unsigned long *key)
{
	longcpy(bkey(geo, node, n), key, geo->keylen);
}

static void setval(struct btree_geo *geo, unsigned long *node, int n,
		   void *val)
{
	node[geo->no_longs + n] = (unsigned long) val;
}

static void clearpair(struct btree_geo *geo, unsigned long *node, int n)
{
	longset(bkey(geo, node, n), 0, geo->keylen);
	node[geo->no_longs + n] = 0;
}

static int keycmp(struct btree_geo *geo, unsigned long *node, int pos,
		  unsigned long *key)
{
	return longcmp(bkey(geo, node, pos), key, geo->keylen);
}

static int keyzero(struct btree_geo *geo, unsigned long *key)
{
	int i;

	for (i = 0; i < geo->keylen; i++)
		if (key[i])
			return 0;

	return 1;
}

static int getpos(struct btree_geo *geo, unsigned long *node,
		unsigned long *key)
{
	int i;

	for (i = 0; i < geo->no_pairs; i++) {
		if (keycmp(geo, node, i, key) <= 0)
			break;
	}
	return i;
}

static int getfill(struct btree_geo *geo, unsigned long *node, int start)
{
	int i;

	for (i = start; i < geo->no_pairs; i++)
		if (!bval(geo, node, i))
			break;
	return i;
}

```

### 修改说明

1. 删减了对更多键类型 (`u32, u64`) 的支持, 本文仅支持 `unsigned long` 类型的键.
2. 将 mempool 相关内存管理替换为 `kmalloc` 相关函数, 不再使用内存池, 现用现申请. 原程序中在内核模块加载时申请了一块缓存 `btree_cachep`, 已删除.
3. 将 `NODESIZE` 改为页大小 `4KB`, 原程序中其大小和 `L1 Cache` 相关. 过于底层.