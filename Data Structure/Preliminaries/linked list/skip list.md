
|             | 红黑树               | B树                            | 跳表                   | AVL树                    |
| ----------- | -------------------- | ------------------------------ | ---------------------- | ------------------------ |
| 查找(平均)  | $O(\log n)$          | $O(\log n)$                    | $O(\log n)$            | $O(\log n)$              |
| 插入(平均)  | $O(\log n)$          | $O(\log n)$                    | $O(\log n)$            | $O(\log n)$              |
| 删除(平均)  | $O(\log n)$          | $O(\log n)$                    | $O(\log n)$            | $O(\log n)$              |
| 空间复杂度  | $O(n)$               | $O(n)$                         | $O(n\log n)$           | $O(n)$                   |
| 算法难度    | 较复杂               | 复杂                           | 简单                   | 较复杂                   |
| 平衡性      | 少量旋转和重着色     | 少量旋转和节点分裂             | 维护索引层             | 旋转                     |
| 平衡因子??? | 1                    | 阶数                           | 随机层数               | 1                        |
| 优点        | 插入和删除快         | 适合大规模数据, 磁盘I/O效率高  | 实现简单, 随时动态变化 | 严格平衡, 查找效率高     |
| 缺点        | 实现复杂, 尤其是插入 | 实现复杂, 尤其是节点合并和分裂 | 需要额外的索引层, 空间开销大                       | 插入和删除慢, 旋转次数多 |

# 跳表

![|500](../../../attach/Pasted%20image%2020240527115938.png)

跳表类似 [B树](../tree/b%20tree.md) + [链表](linked%20list%20vs%20array.md).

```c
/*
 * level 2: 1 ->                     11         
 * level 1: 1 ->      4 ->           11
 * level 0: 1 -> 3 -> 4 -> 5 -> 9 -> 11
 */
#define MAX_LEVEL 16
struct skip_node {
    int key;
    int value;
    struct skip_node *forward[]; /* [level1, level2, ...., leveln] */
};

struct skip_list {
    int level;
    struct skip_node *header;
};

static struct skip_node *new_node(int level, int key, int value)
{
    size_t node_size = sizeof(struct skip_node) + level * sizeof(struct skip_node *);
    struct skip_node *node = (struct skip_node *)malloc(node_size);
    assert(node != NULL)
    node->key = key;
    node->value = value;
    return node;
}

struct skip_list *new(void)
{
    struct skip_list *list = (struct skip_list *)malloc(sizeof(struct skip_list));
    assert(list != NULL);
    list->level = 1;
    list->header = new_node(MAX_LEVEL, 0, 0); // top level node
    for (int i = 0; i < MAX_LEVEL; i++) 
        list->header->forward[i] = NULL;
    return list;
}

```

## search

```c

// 从上到下遍历层
#define for_each_level_reverse(list, level) \
    for (int level = (list)->level - 1; level >= 0; level--)
// 从下到上遍历层
#define for_each_level(list, level) \
    for (int level = 0; level < (list)->level; level++)
// 层内遍历
#define find_key_in_level(pos, level, key) \
    do {\
    while ((pos)->forward[level] && (pos)->forward[level]->key < (key)) {\
        (pos) = (pos)->forward[level]; \
    }} while(0)

struct skip_node *search(struct skip_list *list, int key)
{
    struct skip_node *pos = list->header;
    for_each_level_reverse(list, i) {
        find_key_in_level(pos, i, key);
    }
    pos = pos->forward[0];
    if (pos && pos->key == key)
        return pos;
    return NULL;
}
```

## insert

```c

static int random_level(void)
{
    return rand() % MAX_LEVEL + 1;
}

void insert(struct skip_list *list, int key, int value)
{
    struct skip_node *update[MAX_LEVEL];
    struct skip_node *pos = list->header; 
    for (int i = list->level - 1; i >= 0; i--) {
    for_each_level_reverse(list, i) { 
        find_key_in_level(pos, i, key);
        update[i] = pos;
    }
    pos = pos->forward[0];

    if (pos && pos->key == key) {
        // existed key, update value
        pos->value = value;
    } else {
        int level = random_level();
        if (level > list->level) {
            for (int i = list->level; i < level; i++) {
                update[i] = list->header;
            }
            list->level = level;
        }
        struct skip_node *node = new_node(level, key, value);
        for (int i = 0; i < level; i++) {
            node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = node;
        }
    }
}
```

## delete

```c
void delete(struct skip_list *list, int key)
{
    struct skip_node *update[MAX_LEVEL];
    struct skip_node *pos = list->header;
    for_each_level_reverse(list, i) {
        find_key_in_level(pos, i, key);
        update[i] = pos;
    }
    pos = pos->forward[0];

    if (pos && pos->key == key) {
        for_each_level(list, i) {
            if (update[i]->forward[i] != pos) break;
            update[i]->forward[i] = pos->forward[i];
        }
        free(pos);
        while (list->level > 1 && !list->header->forward[list->level - 1]) {
            list->level--;
        }
    }
}
```


> 代码实现及样例见 [skip_list.c](../../../appendix/程序/skip_list.c)
>
> Redis [Sorted Set](../../Redis/data%20structure/Sorted%20sets.md) 就是使用跳表实现的.