---
author: Joern Engel, Peter Zijlstra
license: GPL2
path: lib/btree.c
---
## Simple In-memory B+Tree

### Main API

B+ Tree 用法和 Linux Radix Tree 类似 (注意, Linux Radix Tree 和教科书的 radix tree 完全不一样). 它们工作良好的前提是, 访问一个随机树节点的速度远快于大量节点内操作. 

当键值空间稀疏时 (sparsely, 25%~50% memory occupation) 时, B+ Tree 比 Radix Tree 更高效, 因为可以减少存储非必需键来适应稀疏数据; 而当键值更密集 (densely) 时, Radix Tree 节点大部分空间皆可以被使用, 更加紧凑地存储键 (98%).

一个教科书没有提及的技巧是: 最小的值被放在了**最右侧**. 节点内所有已使用的凹槽 (指针) 被放到左边, 所有未使用的右侧凹槽则设为 `NULL`. 大部分操作在遍历凹槽时, 会在第一个 `NULL` 位置停下.

```c

#include <linux/btree.h>

#define NODESIZE 4096

// 每个节点的布局如下:
// [key1, key2, ..., keyN] [val1, val2, ..., valN]
struct btree_geo {
	int keylen;   // 
	int no_pairs; // 键值对个数.
	int no_longs; // 每个键都是一个 ulong 数组, no_longs 指该数组大小.
};

struct btree_geo btree_geo32 = {
	.keylen = 1,
	.no_pairs = NODESIZE / sizeof(long) / 2,
	.no_longs = NODESIZE / sizeof(long) / 2,
};

static unsigned long *btree_node_alloc(struct btree_head *head, gfp_t gfp)
{
	unsigned long *node;

	node = kmalloc(NODESIZE, gfp);
	if (likely(node))
		memset(node, 0, NODESIZE);
	return node;
}

void btree_init(struct btree_head *head)
{
	head->node = NULL;
	head->height = 0;
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
	kfree(node);
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
	kfree(right);
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
		kfree(child);
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
		victim->node = NULL;
		victim->height = 0;
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
		kfree(node);
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
	head->node = NULL;
	head->height = 0;
	return count;
}

```

