## 分析
Floyd法复杂度为O(N^3),但胜在实现简洁。
## 具体实现
==warning: include some pseudo-code for compatibility==
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