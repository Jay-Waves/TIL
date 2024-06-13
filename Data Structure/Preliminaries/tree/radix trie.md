基数树

https://en.wikipedia.org/wiki/Radix_tree



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