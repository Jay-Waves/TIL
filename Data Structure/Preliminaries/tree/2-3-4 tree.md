一种 红黑树 的实现模式

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3
#define MAX_CHILDREN 4

struct node {
    int num_keys;
    int keys[MAX_KEYS];
    struct node *children[MAX_CHILDREN];
};

struct tree {
    struct node *root;
};

// 创建一个新节点
static struct node *create_node(void)
{
    struct node *new_node = malloc(sizeof(struct node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->num_keys = 0;
    for (int i = 0; i < MAX_CHILDREN; i++)
        new_node->children[i] = NULL;
    return new_node;
}

// 初始化树
static void tree_init(struct tree *t)
{
    t->root = create_node();
}

// 插入一个键到节点
static void node_insert(struct node *n, int key)
{
    int i;
    for (i = n->num_keys - 1; (i >= 0 && n->keys[i] > key); i--)
        n->keys[i + 1] = n->keys[i];
    n->keys[i + 1] = key;
    n->num_keys++;
}

// 拆分节点
static void split_child(struct node *parent, int index)
{
    struct node *child = parent->children[index];
    struct node *new_child = create_node();

    new_child->num_keys = 1;
    new_child->keys[0] = child->keys[2];

    if (child->children[0]) {
        for (int i = 0; i < 2; i++)
            new_child->children[i] = child->children[i + 2];
    }

    child->num_keys = 1;

    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    parent->children[index + 1] = new_child;
    parent->keys[index] = child->keys[1];
    parent->num_keys++;
}

// 插入一个键到2-3-4树
static void tree_insert(struct tree *t, int key)
{
    struct node *root = t->root;

    if (root->num_keys == MAX_KEYS) {
        struct node *new_root = create_node();
        new_root->children[0] = root;
        split_child(new_root, 0);
        t->root = new_root;
    }

    struct node *current = t->root;
    while (current->children[0]) {
        int i;
        for (i = current->num_keys - 1; (i >= 0 && key < current->keys[i]); i--);
        i++;
        if (current->children[i]->num_keys == MAX_KEYS) {
            split_child(current, i);
            if (key > current->keys[i])
                i++;
        }
        current = current->children[i];
    }

    node_insert(current, key);
}

// 中序遍历并打印树
static void inorder_traversal(struct node *n)
{
    if (!n)
        return;

    for (int i = 0; i < n->num_keys; i++) {
        inorder_traversal(n->children[i]);
        printf("%d ", n->keys[i]);
    }
    inorder_traversal(n->children[n->num_keys]);
}

// 打印树
static void tree_print(struct tree *t)
{
    inorder_traversal(t->root);
    printf("\n");
}

int main(void)
{
    struct tree t;
    tree_init(&t);

    tree_insert(&t, 10);
    tree_insert(&t, 20);
    tree_insert(&t, 5);
    tree_insert(&t, 6);
    tree_insert(&t, 12);
    tree_insert(&t, 30);
    tree_insert(&t, 7);
    tree_insert(&t, 17);

    printf("Inorder traversal of the 2-3-4 tree: ");
    tree_print(&t);

    return 0;
}

```