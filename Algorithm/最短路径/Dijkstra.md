```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// 图的顶点数量
#define V 9

// 查找最小距离节点的函数
static int min_distance(int dist[], bool spt_set[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (!spt_set[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// 打印从源到顶点的距离
static void print_solution(int dist[])
{
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Dijkstra算法实现
static void dijkstra(int graph[V][V], int src)
{
    int dist[V]; // 源节点到其他节点的最短距离
    bool spt_set[V]; // 最短路径树集

    // 初始化所有距离为无穷大，spt_set[] 为 false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, spt_set[i] = false;

    // 源节点到自己的距离为 0
    dist[src] = 0;

    // 找到最短路径
    for (int count = 0; count < V - 1; count++) {
        // 从未处理的顶点中选择距离最小的顶点
        int u = min_distance(dist, spt_set);

        // 将选择的顶点标记为已处理
        spt_set[u] = true;

        // 更新相邻顶点的距离
        for (int v = 0; v < V; v++)
            if (!spt_set[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // 打印最终的最短路径
    print_solution(dist);
}

int main()
{
    // 示例图，graph[i][j] 表示顶点 i 和顶点 j 之间的距离
    int graph[V][V] = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
    };

    // 执行Dijkstra算法，源节点为0
    dijkstra(graph, 0);

    return 0;
}

```