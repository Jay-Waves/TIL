基数树

https://en.wikipedia.org/wiki/Radix_tree

在 radix tree 中，基数 \( r \) 表示每个节点的分支数目上限，即每个内部节点最多可以有多少个子节点。这里的基数 \( r \) 是一个整数，满足 \( r = 2^x \)，其中 \( x \geq 1 \)。因此，基数 \( r \) 实际上是一个节点能够包含的最大子节点数。

具体来说：

- 当 \( r = 2 \) 时，radix tree 是二叉的，每个节点在比较关键字时只比较其中的一位，这种结构可以减少空间使用，但会增加树的深度。
  
- 当 \( r \geq 4 \) 且是2的幂次方时，radix tree 是 r-ary 的，即每个节点最多有 \( r \) 个子节点。这种情况下，radix tree 可以减少树的深度，但可能会增加节点之间的空间占用。

基数 \( r \) 的选择影响了 radix tree 的性能特征，包括树的深度、空间利用效率以及在搜索和插入操作中的效率。因此，在设计和实现 radix tree 时，选择合适的基数 \( r \) 是非常重要的。

In [computer science](https://en.wikipedia.org/wiki/Computer_science "Computer science"), a **radix tree** (also **radix trie** or **compact prefix tree** or **compressed trie**) is a [data structure](https://en.wikipedia.org/wiki/Data_structure "Data structure") that represents a [space-optimized](https://en.wikipedia.org/wiki/Memory_Optimization "Memory Optimization") [trie](https://en.wikipedia.org/wiki/Trie "Trie") (prefix tree) in which each node that is the only child is merged with its parent. The result is that the number of children of every internal node is at most the [radix](https://en.wikipedia.org/wiki/Radix "Radix") r of the radix tree, where r = 2x for some integer x ≥ 1. Unlike regular trees, edges can be labeled with sequences of elements as well as single elements. This makes radix trees much more efficient for small sets (especially if the strings are long) and for sets of strings that share long prefixes.

Unlike regular trees (where whole keys are compared _en masse_ from their beginning up to the point of inequality), the key at each node is compared chunk-of-bits by chunk-of-bits, where the quantity of bits in that chunk at that node is the radix r of the radix trie. When r is 2, the radix trie is binary (i.e., compare that node's 1-bit portion of the key), which minimizes sparseness at the expense of maximizing trie depth—i.e., maximizing up to conflation of nondiverging bit-strings in the key. When r ≥ 4 is a power of 2, then the radix trie is an r-ary trie, which lessens the depth of the radix trie at the expense of potential sparseness.

As an optimization, edge labels can be stored in constant size by using two pointers to a string (for the first and last elements).[[1]](https://en.wikipedia.org/wiki/Radix_tree#cite_note-1)

Note that although the examples in this article show strings as sequences of characters, the type of the string elements can be chosen arbitrarily; for example, as a bit or byte of the string representation when using [multibyte character](https://en.wikipedia.org/wiki/Multibyte_character "Multibyte character") encodings or [Unicode](https://en.wikipedia.org/wiki/Unicode "Unicode").


```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define RADIX_TREE_BITS 4  // 每个节点存储的位数
#define RADIX_TREE_MAX_HEIGHT (32 / RADIX_TREE_BITS)

struct radix_tree_node {
    void *slots[1 << RADIX_TREE_BITS];
};

struct radix_tree_root {
    struct radix_tree_node *rnode;
    int height;
};

// 初始化Radix树
static void radix_tree_init(struct radix_tree_root *root)
{
    root->rnode = NULL;
    root->height = 0;
}

// 创建一个新的Radix树节点
static struct radix_tree_node *radix_tree_node_alloc(void)
{
    struct radix_tree_node *node;
    node = malloc(sizeof(struct radix_tree_node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    memset(node->slots, 0, sizeof(node->slots));
    return node;
}

// 插入一个元素到Radix树
static int radix_tree_insert(struct radix_tree_root *root, unsigned long index, void *item)
{
    struct radix_tree_node **nodep = &root->rnode;
    struct radix_tree_node *node = root->rnode;
    int height = root->height;
    int shift = (height - 1) * RADIX_TREE_BITS;
    int offset;

    // 如果树为空，创建根节点
    if (!node) {
        node = radix_tree_node_alloc();
        root->rnode = node;
        root->height = 1;
        height = 1;
        shift = 0;
    }

    // 查找插入点
    while (height > 0) {
        if (!*nodep) {
            *nodep = radix_tree_node_alloc();
        }
        node = *nodep;
        offset = (index >> shift) & ((1 << RADIX_TREE_BITS) - 1);
        nodep = (struct radix_tree_node **)&node->slots[offset];
        shift -= RADIX_TREE_BITS;
        height--;
    }

    offset = index & ((1 << RADIX_TREE_BITS) - 1);
    node->slots[offset] = item;

    return 0;
}

// 查找一个元素
static void *radix_tree_lookup(struct radix_tree_root *root, unsigned long index)
{
    struct radix_tree_node *node = root->rnode;
    int height = root->height;
    int shift = (height - 1) * RADIX_TREE_BITS;
    int offset;

    if (!node)
        return NULL;

    // 查找路径
    while (height > 0) {
        offset = (index >> shift) & ((1 << RADIX_TREE_BITS) - 1);
        node = (struct radix_tree_node *)node->slots[offset];
        if (!node)
            return NULL;
        shift -= RADIX_TREE_BITS;
        height--;
    }

    offset = index & ((1 << RADIX_TREE_BITS) - 1);
    return node->slots[offset];
}

// 删除一个元素
static void *radix_tree_delete(struct radix_tree_root *root, unsigned long index)
{
    struct radix_tree_node **nodep = &root->rnode;
    struct radix_tree_node *node = root->rnode;
    int height = root->height;
    int shift = (height - 1) * RADIX_TREE_BITS;
    int offset;
    void *item;

    if (!node)
        return NULL;

    // 查找路径
    while (height > 0) {
        offset = (index >> shift) & ((1 << RADIX_TREE_BITS) - 1);
        node = (struct radix_tree_node *)node->slots[offset];
        if (!node)
            return NULL;
        nodep = (struct radix_tree_node **)&node->slots[offset];
        shift -= RADIX_TREE_BITS;
        height--;
    }

    offset = index & ((1 << RADIX_TREE_BITS) - 1);
    item = node->slots[offset];
    node->slots[offset] = NULL;

    return item;
}

// 打印Radix树内容
static void radix_tree_print(struct radix_tree_node *node, int height, unsigned long index)
{
    if (!node)
        return;

    if (height == 0) {
        printf("Index %lu: %p\n", index, node->slots[0]);
    } else {
        for (int i = 0; i < (1 << RADIX_TREE_BITS); i++) {
            if (node->slots[i]) {
                radix_tree_print((struct radix_tree_node *)node->slots[i], height - 1, (index << RADIX_TREE_BITS) | i);
            }
        }
    }
}

int main(void)
{
    struct radix_tree_root root;
    radix_tree_init(&root);

    radix_tree_insert(&root, 1, (void *)100);
    radix_tree_insert(&root, 2, (void *)200);
    radix_tree_insert(&root, 3, (void *)300);
    radix_tree_insert(&root, 15, (void *)1500);

    printf("Radix Tree after insertions:\n");
    radix_tree_print(root.rnode, root.height, 0);

    printf("Lookup index 2: %p\n", radix_tree_lookup(&root, 2));
    printf("Lookup index 15: %p\n", radix_tree_lookup(&root, 15));

    radix_tree_delete(&root, 2);

    printf("Radix Tree after deletion of index 2:\n");
    radix_tree_print(root.rnode, root.height, 0);

    return 0;
}

```