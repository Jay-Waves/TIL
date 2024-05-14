# 例题
- **连通块问题**
题干描述是找链接在一起的#有多少个, 区域大小为 $A_{m\times n}$. 属于dfs初级问题

```c
#include<iostream>
using namespace std;
int n, m;
int ans;
char grass[110][110];
int vis[110][110];
int nx[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
bool in(int x, int y) {
    return x >= 1 && x <= n && y >= 1 && y <= m;
}
 
void dfs(int x, int y) {
    for (int i = 0; i <= 3; i++) {
	int nowx = x + nx[i][0];
	int nowy = y + nx[i][1];
	if (in(nowx, nowy) && grass[nowx][nowy] == '#' && !vis[nowx][nowy]) {
	    vis[nowx][nowy] = 1;
	    dfs(nowx, nowy);
	}
    }
}
 
int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
	for (int j = 1; j <= m; j++) {
	    cin >> grass[i][j];
	}
    }
    for (int i = 1; i <= n; i++) {
	for (int j = 1; j <= m; j++) {
	    if (grass[i][j] == '#' && !vis[i][j]) {
		ans++;
		vis[i][j] = 1;//代码这里有点问题
		dfs(i, j);
	    }
	}
    }
    cout << ans;
    return 0;
}
```