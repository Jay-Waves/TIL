#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_LEVEL 16
// using these macros after C99
#define for_each_level_reverse(list, level) \
    for (int level = (list)->level - 1; level >= 0; level--)
#define for_each_level(list, level) \
    for (int level = 0; level < (list)->level; level++)
#define find_key_in_level(pos, level, key) \
    do {\
    while ((pos)->forward[level] && (pos)->forward[level]->key < (key)) {\
        (pos) = (pos)->forward[level]; \
    }} while(0)

/*
 * level 2: 1 ->                     11         
 * level 1: 1 ->      4 ->           11
 * level 0: 1 -> 3 -> 4 -> 5 -> 9 -> 11
 */

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
    if (!node) exit(EXIT_FAILURE);
    node->key = key;
    node->value = value;
    return node;
}

struct skip_list *new(void)
{
    struct skip_list *list = (struct skip_list *)malloc(sizeof(struct skip_list));
    if (!list)  exit(EXIT_FAILURE);
    list->level = 1;
    list->header = new_node(MAX_LEVEL, 0, 0); // top level node
    for (int i = 0; i < MAX_LEVEL; i++) 
        list->header->forward[i] = NULL;
    return list;
}

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

void print(struct skip_list *list)
{
    printf("Skip List:\n");
    for_each_level_reverse(list, i) {
        struct skip_node *pos = list->header->forward[i];
        printf("Level %d: ", i);
        while (pos) {
            printf("(%d, %d) ", pos->key, pos->value);
            pos = pos->forward[i];
        }
        printf("\n");
    }
}

int main(void)
{
    struct skip_list *list = new();

    insert(list, 3, 30);
    insert(list, 6, 60);
    insert(list, 7, 70);
    insert(list, 9, 90);
    insert(list, 12, 120);
    insert(list, 19, 190);
    insert(list, 17, 170);

    print(list);

    struct skip_node *node = search(list, 9);
    if (node)
        printf("Found node with key 9 and value %d\n", node->value);
    else
        printf("Node with key 9 not found\n");

    delete(list, 9);
    print(list);

    return 0;
}
