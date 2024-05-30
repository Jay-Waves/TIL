```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

// 定义节点结构
struct node {
    int x, y;               // 节点坐标
    int g, h, f;            // g: 从起点到当前节点的代价, h: 从当前节点到终点的估算代价, f = g + h
    struct node *parent;    // 父节点指针
    struct node *next;      // 用于链表
};

// 定义开放列表和关闭列表
struct node *open_list = NULL;
struct node *closed_list = NULL;

// 节点比较函数
static bool node_cmp(struct node *a, struct node *b)
{
    return a->x == b->x && a->y == b->y;
}

// 计算曼哈顿距离
static int manhattan(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// 从列表中移除节点
static void list_remove(struct node **list, struct node *n)
{
    if (*list == n) {
        *list = n->next;
    } else {
        struct node *current = *list;
        while (current->next && current->next != n) {
            current = current->next;
        }
        if (current->next) {
            current->next = n->next;
        }
    }
}

// 将节点插入列表
static void list_add(struct node **list, struct node *n)
{
    n->next = *list;
    *list = n;
}

// 检查节点是否在列表中
static struct node *list_find(struct node *list, struct node *n)
{
    while (list) {
        if (node_cmp(list, n)) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

// 创建节点
static struct node *node_create(int x, int y, struct node *parent, int g, int h)
{
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (!n) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->x = x;
    n->y = y;
    n->g = g;
    n->h = h;
    n->f = g + h;
    n->parent = parent;
    n->next = NULL;
    return n;
}

// 打印路径
static void print_path(struct node *n)
{
    if (n->parent) {
        print_path(n->parent);
    }
    printf("(%d, %d)\n", n->x, n->y);
}

// A*算法
void a_star(int start_x, int start_y, int goal_x, int goal_y, int width, int height, bool (*is_walkable)(int, int))
{
    struct node *start = node_create(start_x, start_y, NULL, 0, manhattan(start_x, start_y, goal_x, goal_y));
    list_add(&open_list, start);

    while (open_list) {
        // 找到开放列表中f值最小的节点
        struct node *current = open_list;
        struct node *min_f_node = current;
        while (current) {
            if (current->f < min_f_node->f) {
                min_f_node = current;
            }
            current = current->next;
        }

        // 如果找到目标节点，打印路径并退出
        if (min_f_node->x == goal_x && min_f_node->y == goal_y) {
            print_path(min_f_node);
            return;
        }

        // 将当前节点从开放列表移除，加入关闭列表
        list_remove(&open_list, min_f_node);
        list_add(&closed_list, min_f_node);

        // 处理当前节点的邻居节点
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};
        for (int i = 0; i < 4; i++) {
            int new_x = min_f_node->x + dx[i];
            int new_y = min_f_node->y + dy[i];
            if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height || !is_walkable(new_x, new_y)) {
                continue;
            }

            struct node *neighbor = node_create(new_x, new_y, min_f_node, min_f_node->g + 1, manhattan(new_x, new_y, goal_x, goal_y));
            if (list_find(closed_list, neighbor)) {
                free(neighbor);
                continue;
            }

            struct node *existing_node = list_find(open_list, neighbor);
            if (existing_node) {
                if (neighbor->g < existing_node->g) {
                    existing_node->g = neighbor->g;
                    existing_node->f = neighbor->f;
                    existing_node->parent = min_f_node;
                }
                free(neighbor);
            } else {
                list_add(&open_list, neighbor);
            }
        }
    }

    printf("No path found\n");
}

// 判断节点是否可行走（示例函数，可根据实际情况调整）
bool is_walkable(int x, int y)
{
    // 这里简单示例所有节点都可行走
    return true;
}

int main()
{
    int start_x = 0, start_y = 0;
    int goal_x = 5, goal_y = 5;
    int width = 10, height = 10;

    a_star(start_x, start_y, goal_x, goal_y, width, height, is_walkable);

    return 0;
}

```