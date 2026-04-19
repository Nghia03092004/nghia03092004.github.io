// IOI 1991 - Problem 1: Island
// Count islands (connected components of 1s) in a 2D grid via BFS.
#include <bits/stdc++.h>
using namespace std;

int R, C;
int grid[505][505];
bool vis[505][505];
const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

void bfs(int sr, int sc) {
    queue<pair<int,int>> q;
    q.push({sr, sc});
    vis[sr][sc] = true;
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C
                && !vis[nr][nc] && grid[nr][nc] == 1) {
                vis[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
}

int main() {
    scanf("%d%d", &R, &C);
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            scanf("%d", &grid[i][j]);

    memset(vis, false, sizeof(vis));
    int islands = 0;
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (grid[i][j] == 1 && !vis[i][j]) {
                islands++;
                bfs(i, j);
            }

    printf("%d\n", islands);
    return 0;
}
