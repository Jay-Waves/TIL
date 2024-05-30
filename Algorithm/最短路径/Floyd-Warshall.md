## Floyd-Warshall Algorithm

弗洛伊德算法用于计算图中所有顶点之间最短路径的算法. 算法复杂度为 $O(n^3)$, $n$ 为图顶点数, 理解起来很直观.

```c
#include <stdio.h>
#include <stdlib.h>

#define INF 99999
#define V 4  // 定义图中的顶点数量

// 打印解决方案
static void print_solution(int dist[][V])
{
    printf("The following matrix shows the shortest distances between every pair of vertices\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

// 实现弗洛伊德算法
static void floyd_warshall(int graph[][V])
{
    int dist[V][V], i, j, k;

    // 初始化距离矩阵
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // 使用弗洛伊德算法
    for (k = 0; k < V; k++) {
        // 将顶点 k 作为中间顶点
        for (i = 0; i < V; i++) {
            // 从顶点 i 到顶点 j 的所有顶点对
            for (j = 0; j < V; j++) {
                // 如果顶点 k 是从 i 到 j 的最短路径中间的一个顶点，则更新 dist[i][j]
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // 打印最短路径结果
    print_solution(dist);
}

int main()
{
    /* 表示以下的图
        10
    (0)------->(3)
     |         /|\
     5|          |
     |          | 1
    \|/         |
    (1)------->(2)
        3           */
    int graph[V][V] = {{0,   5,  INF, 10},
                       {INF, 0,   3, INF},
                       {INF, INF, 0,   1},
                       {INF, INF, INF, 0}
                      };

    // 运行弗洛伊德算法
    floyd_warshall(graph);

    return 0;
}

```

可以有负权边, 不能有负权环

自己的实现:
```c
int data[100][100];//图-邻接矩阵
int graph[100][100];//存两节点间相对最短路径长度(weight)
int path[100][100];//存中转节点

/*初始化函数，初始化Floyd法*/
void Init( int v_num ) {
    int i, j;
    int out, in;
    //init graph and path
    //existed situation, pseudo-code
	for i from 0 to v_num, in step of 1:
		for j from 0 to v_num, in step of 1:
			if (vertex_i directly links vertex_j)
				graph[i][j] = data[i][j];

    //unexisted situation,  c
    for ( i = 0; i < v_num; i++ ) {
        for ( j = 0; j < v_num; j++ ) {
            path[i][j] = -1;
            if ( !graph[i][j] ) {
                if ( i == j ) {
                    graph[i][j] = 0;
                }
                else
                    graph[i][j] = 1e6;//infinity
            }
        }
    }
}

/*Floyd主体，思想是动态规划思想：前面的不影响后面的*/
void Floyd( int v_num ) {
    //若是无向图， 理论上只需要遍历即可上三角矩阵即可
    int out, in, k;
    Init( v_num );//初始化graph权值表
    for ( k = 0; k < v_num; k++ ) {//不断添加中转点
        for ( out = 0; out < v_num; out++ ) {
            for ( in = 0; in < v_num; in++ ) {
                if ( graph[out][in] > graph[out][k] + graph[k][in] ) {
                    graph[out][in] = graph[out][k] + graph[k][in];//更新最短路径长度
                    path[out][in] = k;//中转点, 最靠近终点的位置
                }
            }
        }
    }
}

/*二分递归，读取完整最短路径, 注意对重复情况的处理*/
int real_path[100];//全部初始化为-1
int path_len;
void ReadPath( int origin, int dest ) {
    int i;
    if ( origin == -1 ) {
        for ( i = 0; i < path_len; i++ ) 
            if ( real_path[i] == dest )//已经存在，不再写入
                return;
        real_path[path_len++] = dest;
        return;
    }
    else if ( dest == -1 ) {
        for ( i = 0; i < path_len; i++ ) 
            if ( real_path[i] == origin )
                return;
        real_path[path_len++] = origin;
        return;
    }
    ReadPath( origin, path[origin][dest] );
    ReadPath( path[origin][dest], dest );
}
```