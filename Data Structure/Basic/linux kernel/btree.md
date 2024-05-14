---
author: Joern Engel, Peter Zijlstra
license: GPL2
path: lib/btree.c,include/linux/btree.h
---

## Simple In-memory B+Tree

```c
#ifndef BTREE_H
#define BTREE_H

/**
 * DOC: B+Tree basics
 *
 * A B+Tree is a data structure for looking up arbitrary (currently allowing
 * unsigned long, u32, u64 and 2 * u64) keys into pointers. The data structure
 * is described at https://en.wikipedia.org/wiki/B-tree, we currently do not
 * use binary search to find the key on lookups.
 *
 * Each B+Tree consists of a head, that contains bookkeeping information and
 * a variable number (starting with zero) nodes. Each node contains the keys
 * and pointers to sub-nodes, or, for leaf nodes, the keys and values for the
 * tree entries.
 *
 * Each node in this implementation has the following layout:
 * [key1, key2, ..., keyN] [val1, val2, ..., valN]
 *
 * Each key here is an array of unsigned longs, geo->no_longs in total. The
 * number of keys and values (N) is geo->no_pairs.
 */

/**
 * struct btree_head - btree head
 *
 * @node: the first node in the tree
 * @mempool: mempool used for node allocations
 * @height: current of the tree
 */
struct btree_head {
	unsigned long *node;
	mempool_t *mempool;
	int height;
};

/* btree geometry */
struct btree_geo;

/**
 * btree_alloc - allocate function for the mempool
 * @gfp_mask: gfp mask for the allocation
 * @pool_data: unused
 */
void *btree_alloc(gfp_t gfp_mask, void *pool_data);

/**
 * btree_free - free function for the mempool
 * @element: the element to free
 * @pool_data: unused
 */
void btree_free(void *element, void *pool_data);

/**
 * btree_init_mempool - initialise a btree with given mempool
 *
 * @head: the btree head to initialise
 * @mempool: the mempool to use
 *
 * When this function is used, there is no need to destroy
 * the mempool.
 */
void btree_init_mempool(struct btree_head *head, mempool_t *mempool);

/**
 * btree_init - initialise a btree
 *
 * @head: the btree head to initialise
 *
 * This function allocates the memory pool that the
 * btree needs. Returns zero or a negative error code
 * (-%ENOMEM) when memory allocation fails.
 *
 */
int __must_check btree_init(struct btree_head *head);

/**
 * btree_destroy - destroy mempool
 *
 * @head: the btree head to destroy
 *
 * This function destroys the internal memory pool, use only
 * when using btree_init(), not with btree_init_mempool().
 */
void btree_destroy(struct btree_head *head);

/**
 * btree_lookup - look up a key in the btree
 *
 * @head: the btree to look in
 * @geo: the btree geometry
 * @key: the key to look up
 *
 * This function returns the value for the given key, or %NULL.
 */
void *btree_lookup(struct btree_head *head, struct btree_geo *geo,
		   unsigned long *key);

/**
 * btree_insert - insert an entry into the btree
 *
 * @head: the btree to add to
 * @geo: the btree geometry
 * @key: the key to add (must not already be present)
 * @val: the value to add (must not be %NULL)
 * @gfp: allocation flags for node allocations
 *
 * This function returns 0 if the item could be added, or an
 * error code if it failed (may fail due to memory pressure).
 */
int __must_check btree_insert(struct btree_head *head, struct btree_geo *geo,
			      unsigned long *key, void *val, gfp_t gfp);
/**
 * btree_update - update an entry in the btree
 *
 * @head: the btree to update
 * @geo: the btree geometry
 * @key: the key to update
 * @val: the value to change it to (must not be %NULL)
 *
 * This function returns 0 if the update was successful, or
 * -%ENOENT if the key could not be found.
 */
int btree_update(struct btree_head *head, struct btree_geo *geo,
		 unsigned long *key, void *val);
/**
 * btree_remove - remove an entry from the btree
 *
 * @head: the btree to update
 * @geo: the btree geometry
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
 * @geo: the btree geometry
 * @gfp: allocation flags
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
 * @geo: btree geometry
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
 * @geo: btree geometry
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


#include <linux/btree-128.h>

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

#define BTREE_TYPE_SUFFIX 32
#define BTREE_TYPE_BITS 32
#define BTREE_TYPE_GEO &btree_geo32
#define BTREE_KEYTYPE u32
#include <linux/btree-type.h>

#define btree_for_each_safe32(head, key, val)	\
	for (val = btree_last32(head, &key);	\
	     val;				\
	     val = btree_get_prev32(head, &key))

extern struct btree_geo btree_geo64;
#define BTREE_TYPE_SUFFIX 64
#define BTREE_TYPE_BITS 64
#define BTREE_TYPE_GEO &btree_geo64
#define BTREE_KEYTYPE u64
#include <linux/btree-type.h>

#define btree_for_each_safe64(head, key, val)	\
	for (val = btree_last64(head, &key);	\
	     val;				\
	     val = btree_get_prev64(head, &key))

#endif
```

`lib/btree.c`

```c
/*
 * 
 *
 * A relatively simple B+Tree implementation.  I have written it as a learning
 * exercise to understand how B+Trees work.  Turned out to be useful as well.
 *
 * B+Trees can be used similar to Linux radix trees (which don't have anything
 * in common with textbook radix trees, beware).  Prerequisite for them working
 * well is that access to a random tree node is much faster than a large number
 * of operations within each node.
 *
 * Disks have fulfilled the prerequisite for a long time.  More recently DRAM
 * has gained similar properties, as memory access times, when measured in cpu
 * cycles, have increased.  Cacheline sizes have increased as well, which also
 * helps B+Trees.
 *
 * Compared to radix trees, B+Trees are more efficient when dealing with a
 * sparsely populated address space.  Between 25% and 50% of the memory is
 * occupied with valid pointers.  When densely populated, radix trees contain
 * ~98% pointers - hard to beat.  Very sparse radix trees contain only ~2%
 * pointers.
 *
 * This particular implementation stores pointers identified by a long value.
 * Storing NULL pointers is illegal, lookup will return NULL when no entry
 * was found.
 *
 * A tricks was used that is not commonly found in textbooks.  The lowest
 * values are to the right, not to the left.  All used slots within a node
 * are on the left, all unused slots contain NUL values.  Most operations
 * simply loop once over all slots and terminate on the first NUL.
 */

#include <linux/btree.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define NODESIZE MAX(L1_CACHE_BYTES, 128)

struct btree_geo {
	int keylen;
	int no_pairs;
	int no_longs;
};

struct btree_geo btree_geo32 = {
	.keylen = 1,
	.no_pairs = NODESIZE / sizeof(long) / 2,
	.no_longs = NODESIZE / sizeof(long) / 2,
};

#define LONG_PER_U64 (64 / BITS_PER_LONG)
struct btree_geo btree_geo64 = {
	.keylen = LONG_PER_U64,
	.no_pairs = NODESIZE / sizeof(long) / (1 + LONG_PER_U64),
	.no_longs = LONG_PER_U64 * (NODESIZE / sizeof(long) / (1 + LONG_PER_U64)),
};

#define MAX_KEYLEN	(2 * LONG_PER_U64)

static struct kmem_cache *btree_cachep;

void *btree_alloc(gfp_t gfp_mask, void *pool_data)
{
	return kmem_cache_alloc(btree_cachep, gfp_mask);
}

void btree_free(void *element, void *pool_data)
{
	kmem_cache_free(btree_cachep, element);
}

static unsigned long *btree_node_alloc(struct btree_head *head, gfp_t gfp)
{
	unsigned long *node;

	node = mempool_alloc(head->mempool, gfp);
	if (likely(node))
		memset(node, 0, NODESIZE);
	return node;
}

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

static inline void __btree_init(struct btree_head *head)
{
	head->node = NULL;
	head->height = 0;
}

void btree_init_mempool(struct btree_head *head, mempool_t *mempool)
{
	__btree_init(head);
	head->mempool = mempool;
}

int btree_init(struct btree_head *head)
{
	__btree_init(head);
	head->mempool = mempool_create(0, btree_alloc, btree_free, NULL);
	if (!head->mempool)
		return -ENOMEM;
	return 0;
}

void btree_destroy(struct btree_head *head)
{
	mempool_free(head->node, head->mempool);
	mempool_destroy(head->mempool);
	head->mempool = NULL;
}

void *btree_last(struct btree_head *head, struct btree_geo *geo,
		 unsigned long *key)
{
	int height = head->height;
	unsigned long *node = head->node;

	if (height == 0)
		return NULL;

	for ( ; height > 1; height--)
		node = bval(geo, node, 0);

	longcpy(key, bkey(geo, node, 0), geo->keylen);
	return bval(geo, node, 0);
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

void *btree_lookup(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key)
{
	int i, height = head->height;
	unsigned long *node = head->node;

	if (height == 0)
		return NULL;

	for ( ; height > 1; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;
		if (i == geo->no_pairs)
			return NULL;
		node = bval(geo, node, i);
		if (!node)
			return NULL;
	}

	if (!node)
		return NULL;

	for (i = 0; i < geo->no_pairs; i++)
		if (keycmp(geo, node, i, key) == 0)
			return bval(geo, node, i);
	return NULL;
}

int btree_update(struct btree_head *head, struct btree_geo *geo,
		 unsigned long *key, void *val)
{
	int i, height = head->height;
	unsigned long *node = head->node;

	if (height == 0)
		return -ENOENT;

	for ( ; height > 1; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;
		if (i == geo->no_pairs)
			return -ENOENT;
		node = bval(geo, node, i);
		if (!node)
			return -ENOENT;
	}

	if (!node)
		return -ENOENT;

	for (i = 0; i < geo->no_pairs; i++)
		if (keycmp(geo, node, i, key) == 0) {
			setval(geo, node, i, val);
			return 0;
		}
	return -ENOENT;
}

/*
 * Usually this function is quite similar to normal lookup.  But the key of
 * a parent node may be smaller than the smallest key of all its siblings.
 * In such a case we cannot just return NULL, as we have only proven that no
 * key smaller than __key, but larger than this parent key exists.
 * So we set __key to the parent key and retry.  We have to use the smallest
 * such parent key, which is the last parent key we encountered.
 */
void *btree_get_prev(struct btree_head *head, struct btree_geo *geo,
		     unsigned long *__key)
{
	int i, height;
	unsigned long *node, *oldnode;
	unsigned long *retry_key = NULL, key[MAX_KEYLEN];

	if (keyzero(geo, __key))
		return NULL;

	if (head->height == 0)
		return NULL;
	longcpy(key, __key, geo->keylen);
retry:
	dec_key(geo, key);

	node = head->node;
	for (height = head->height ; height > 1; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;
		if (i == geo->no_pairs)
			goto miss;
		oldnode = node;
		node = bval(geo, node, i);
		if (!node)
			goto miss;
		retry_key = bkey(geo, oldnode, i);
	}

	if (!node)
		goto miss;

	for (i = 0; i < geo->no_pairs; i++) {
		if (keycmp(geo, node, i, key) <= 0) {
			if (bval(geo, node, i)) {
				longcpy(__key, bkey(geo, node, i), geo->keylen);
				return bval(geo, node, i);
			} else
				goto miss;
		}
	}
miss:
	if (retry_key) {
		longcpy(key, retry_key, geo->keylen);
		retry_key = NULL;
		goto retry;
	}
	return NULL;
}
EXPORT_SYMBOL_GPL(btree_get_prev);

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

/*
 * locate the correct leaf node in the btree
 */
static unsigned long *find_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level)
{
	unsigned long *node = head->node;
	int i, height;

	for (height = head->height; height > level; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;

		if ((i == geo->no_pairs) || !bval(geo, node, i)) {
			/* right-most key is too large, update it */
			/* FIXME: If the right-most key on higher levels is
			 * always zero, this wouldn't be necessary. */
			i--;
			setkey(geo, node, i, key);
		}
		BUG_ON(i < 0);
		node = bval(geo, node, i);
	}
	BUG_ON(!node);
	return node;
}

static int btree_grow(struct btree_head *head, struct btree_geo *geo,
		      gfp_t gfp)
{
	unsigned long *node;
	int fill;

	node = btree_node_alloc(head, gfp);
	if (!node)
		return -ENOMEM;
	if (head->node) {
		fill = getfill(geo, head->node, 0);
		setkey(geo, node, 0, bkey(geo, head->node, fill - 1));
		setval(geo, node, 0, head->node);
	}
	head->node = node;
	head->height++;
	return 0;
}

static void btree_shrink(struct btree_head *head, struct btree_geo *geo)
{
	unsigned long *node;
	int fill;

	if (head->height <= 1)
		return;

	node = head->node;
	fill = getfill(geo, node, 0);
	BUG_ON(fill > 1);
	head->node = bval(geo, node, 0);
	head->height--;
	mempool_free(node, head->mempool);
}

static int btree_insert_level(struct btree_head *head, struct btree_geo *geo,
			      unsigned long *key, void *val, int level,
			      gfp_t gfp)
{
	unsigned long *node;
	int i, pos, fill, err;

	BUG_ON(!val);
	if (head->height < level) {
		err = btree_grow(head, geo, gfp);
		if (err)
			return err;
	}

retry:
	node = find_level(head, geo, key, level);
	pos = getpos(geo, node, key);
	fill = getfill(geo, node, pos);
	/* two identical keys are not allowed */
	BUG_ON(pos < fill && keycmp(geo, node, pos, key) == 0);

	if (fill == geo->no_pairs) {
		/* need to split node */
		unsigned long *new;

		new = btree_node_alloc(head, gfp);
		if (!new)
			return -ENOMEM;
		err = btree_insert_level(head, geo,
				bkey(geo, node, fill / 2 - 1),
				new, level + 1, gfp);
		if (err) {
			mempool_free(new, head->mempool);
			return err;
		}
		for (i = 0; i < fill / 2; i++) {
			setkey(geo, new, i, bkey(geo, node, i));
			setval(geo, new, i, bval(geo, node, i));
			setkey(geo, node, i, bkey(geo, node, i + fill / 2));
			setval(geo, node, i, bval(geo, node, i + fill / 2));
			clearpair(geo, node, i + fill / 2);
		}
		if (fill & 1) {
			setkey(geo, node, i, bkey(geo, node, fill - 1));
			setval(geo, node, i, bval(geo, node, fill - 1));
			clearpair(geo, node, fill - 1);
		}
		goto retry;
	}
	BUG_ON(fill >= geo->no_pairs);

	/* shift and insert */
	for (i = fill; i > pos; i--) {
		setkey(geo, node, i, bkey(geo, node, i - 1));
		setval(geo, node, i, bval(geo, node, i - 1));
	}
	setkey(geo, node, pos, key);
	setval(geo, node, pos, val);

	return 0;
}

int btree_insert(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, void *val, gfp_t gfp)
{
	BUG_ON(!val);
	return btree_insert_level(head, geo, key, val, 1, gfp);
}

static void *btree_remove_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level);
static void merge(struct btree_head *head, struct btree_geo *geo, int level,
		unsigned long *left, int lfill,
		unsigned long *right, int rfill,
		unsigned long *parent, int lpos)
{
	int i;

	for (i = 0; i < rfill; i++) {
		/* Move all keys to the left */
		setkey(geo, left, lfill + i, bkey(geo, right, i));
		setval(geo, left, lfill + i, bval(geo, right, i));
	}
	/* Exchange left and right child in parent */
	setval(geo, parent, lpos, right);
	setval(geo, parent, lpos + 1, left);
	/* Remove left (formerly right) child from parent */
	btree_remove_level(head, geo, bkey(geo, parent, lpos), level + 1);
	mempool_free(right, head->mempool);
}

static void rebalance(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level, unsigned long *child, int fill)
{
	unsigned long *parent, *left = NULL, *right = NULL;
	int i, no_left, no_right;

	if (fill == 0) {
		/* Because we don't steal entries from a neighbour, this case
		 * can happen.  Parent node contains a single child, this
		 * node, so merging with a sibling never happens.
		 */
		btree_remove_level(head, geo, key, level + 1);
		mempool_free(child, head->mempool);
		return;
	}

	parent = find_level(head, geo, key, level + 1);
	i = getpos(geo, parent, key);
	BUG_ON(bval(geo, parent, i) != child);

	if (i > 0) {
		left = bval(geo, parent, i - 1);
		no_left = getfill(geo, left, 0);
		if (fill + no_left <= geo->no_pairs) {
			merge(head, geo, level,
					left, no_left,
					child, fill,
					parent, i - 1);
			return;
		}
	}
	if (i + 1 < getfill(geo, parent, i)) {
		right = bval(geo, parent, i + 1);
		no_right = getfill(geo, right, 0);
		if (fill + no_right <= geo->no_pairs) {
			merge(head, geo, level,
					child, fill,
					right, no_right,
					parent, i);
			return;
		}
	}
	/*
	 * We could also try to steal one entry from the left or right
	 * neighbor.  By not doing so we changed the invariant from
	 * "all nodes are at least half full" to "no two neighboring
	 * nodes can be merged".  Which means that the average fill of
	 * all nodes is still half or better.
	 */
}

static void *btree_remove_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level)
{
	unsigned long *node;
	int i, pos, fill;
	void *ret;

	if (level > head->height) {
		/* we recursed all the way up */
		head->height = 0;
		head->node = NULL;
		return NULL;
	}

	node = find_level(head, geo, key, level);
	pos = getpos(geo, node, key);
	fill = getfill(geo, node, pos);
	if ((level == 1) && (keycmp(geo, node, pos, key) != 0))
		return NULL;
	ret = bval(geo, node, pos);

	/* remove and shift */
	for (i = pos; i < fill - 1; i++) {
		setkey(geo, node, i, bkey(geo, node, i + 1));
		setval(geo, node, i, bval(geo, node, i + 1));
	}
	clearpair(geo, node, fill - 1);

	if (fill - 1 < geo->no_pairs / 2) {
		if (level < head->height)
			rebalance(head, geo, key, level, node, fill - 1);
		else if (fill - 1 == 1)
			btree_shrink(head, geo);
	}

	return ret;
}

void *btree_remove(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key)
{
	if (head->height == 0)
		return NULL;

	return btree_remove_level(head, geo, key, 1);
}
EXPORT_SYMBOL_GPL(btree_remove);

int btree_merge(struct btree_head *target, struct btree_head *victim,
		struct btree_geo *geo, gfp_t gfp)
{
	unsigned long key[MAX_KEYLEN];
	unsigned long dup[MAX_KEYLEN];
	void *val;
	int err;

	BUG_ON(target == victim);

	if (!(target->node)) {
		/* target is empty, just copy fields over */
		target->node = victim->node;
		target->height = victim->height;
		__btree_init(victim);
		return 0;
	}

	/* TODO: This needs some optimizations.  Currently we do three tree
	 * walks to remove a single object from the victim.
	 */
	for (;;) {
		if (!btree_last(victim, geo, key))
			break;
		val = btree_lookup(victim, geo, key);
		err = btree_insert(target, geo, key, val, gfp);
		if (err)
			return err;
		/* We must make a copy of the key, as the original will get
		 * mangled inside btree_remove. */
		longcpy(dup, key, geo->keylen);
		btree_remove(victim, geo, dup);
	}
	return 0;
}

static size_t __btree_for_each(struct btree_head *head, struct btree_geo *geo,
			       unsigned long *node, unsigned long opaque,
			       void (*func)(void *elem, unsigned long opaque,
					    unsigned long *key, size_t index,
					    void *func2),
			       void *func2, int reap, int height, size_t count)
{
	int i;
	unsigned long *child;

	for (i = 0; i < geo->no_pairs; i++) {
		child = bval(geo, node, i);
		if (!child)
			break;
		if (height > 1)
			count = __btree_for_each(head, geo, child, opaque,
					func, func2, reap, height - 1, count);
		else
			func(child, opaque, bkey(geo, node, i), count++,
					func2);
	}
	if (reap)
		mempool_free(node, head->mempool);
	return count;
}

static void empty(void *elem, unsigned long opaque, unsigned long *key,
		  size_t index, void *func2)
{
}

void visitorl(void *elem, unsigned long opaque, unsigned long *key,
	      size_t index, void *__func)
{
	visitorl_t func = __func;

	func(elem, opaque, *key, index);
}

void visitor32(void *elem, unsigned long opaque, unsigned long *__key,
	       size_t index, void *__func)
{
	visitor32_t func = __func;
	u32 *key = (void *)__key;

	func(elem, opaque, *key, index);
}

void visitor64(void *elem, unsigned long opaque, unsigned long *__key,
	       size_t index, void *__func)
{
	visitor64_t func = __func;
	u64 *key = (void *)__key;

	func(elem, opaque, *key, index);
}

size_t btree_visitor(struct btree_head *head, struct btree_geo *geo,
		     unsigned long opaque,
		     void (*func)(void *elem, unsigned long opaque,
		     		  unsigned long *key,
		     		  size_t index, void *func2),
		     void *func2)
{
	size_t count = 0;

	if (!func2)
		func = empty;
	if (head->node)
		count = __btree_for_each(head, geo, head->node, opaque, func,
				func2, 0, head->height, 0);
	return count;
}

size_t btree_grim_visitor(struct btree_head *head, struct btree_geo *geo,
			  unsigned long opaque,
			  void (*func)(void *elem, unsigned long opaque,
				       unsigned long *key,
				       size_t index, void *func2),
			  void *func2)
{
	size_t count = 0;

	if (!func2)
		func = empty;
	if (head->node)
		count = __btree_for_each(head, geo, head->node, opaque, func,
				func2, 1, head->height, 0);
	__btree_init(head);
	return count;
}

static int __init btree_module_init(void)
{
	btree_cachep = kmem_cache_create("btree_node", NODESIZE, 0,
			SLAB_HWCACHE_ALIGN, NULL);
	return 0;
}

static void __exit btree_module_exit(void)
{
	kmem_cache_destroy(btree_cachep);
}

/* If core code starts using btree, initialization should happen even earlier */
module_init(btree_module_init);
module_exit(btree_module_exit);

```