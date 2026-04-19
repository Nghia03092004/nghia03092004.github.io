// IOI 1998 - Camelot
// BFS knight distances + enumerate gathering point and pickup point for king
// Time: O(S^2 + S^2 * n), Space: O(S^2) where S = R*C
#include <bits/stdc++.h>
using namespace std;

const int MAXR = 30, MAXC = 26;
const int INF = 1000000;
int R, C;
int kdist[MAXR * MAXC][MAXR * MAXC];
int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};

int idx(int r, int c) { return r * C + c; }

void bfs(int src) {
    queue<int> q;
    q.push(src);
    kdist[src][src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int r = u / C, c = u % C;
        for (int d = 0; d < 8; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            int v = idx(nr, nc);
            if (kdist[src][v] == -1) {
                kdist[src][v] = kdist[src][u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    scanf("%d %d", &R, &C);
    int total = R * C;

    // Read king position
    char col;
    int row;
    scanf(" %c %d", &col, &row);
    int kingR = row - 1, kingC = col - 'A';

    // Read knight positions
    vector<int> knights;
    while (scanf(" %c %d", &col, &row) == 2) {
        int kr = row - 1, kc = col - 'A';
        knights.push_back(idx(kr, kc));
    }
    int numKnights = (int)knights.size();

    // Handle no knights: king stays put
    if (numKnights == 0) {
        printf("0\n");
        return 0;
    }

    // BFS from every square for knight distances
    memset(kdist, -1, sizeof(kdist));
    for (int i = 0; i < total; i++)
        bfs(i);

    // Replace unreachable with large value
    for (int i = 0; i < total; i++)
        for (int j = 0; j < total; j++)
            if (kdist[i][j] == -1)
                kdist[i][j] = INF;

    int ans = INT_MAX;

    // Try each gathering point
    for (int g = 0; g < total; g++) {
        // Sum of knight distances to g
        int base = 0;
        bool reachable = true;
        for (int i = 0; i < numKnights; i++) {
            if (kdist[knights[i]][g] >= INF) { reachable = false; break; }
            base += kdist[knights[i]][g];
        }
        if (!reachable) continue;

        int gr = g / C, gc = g % C;

        // Option 1: king walks alone to gathering point
        int kingAlone = base + max(abs(kingR - gr), abs(kingC - gc));
        ans = min(ans, kingAlone);

        // Option 2: one knight picks up king at some point p
        for (int i = 0; i < numKnights; i++) {
            int saved = kdist[knights[i]][g];
            // Try all squares as pickup points
            for (int p = 0; p < total; p++) {
                int pr = p / C, pc = p % C;
                int kingWalk = max(abs(kingR - pr), abs(kingC - pc));
                int knightDetour = kdist[knights[i]][p] + kdist[p][g];
                if (knightDetour >= INF) continue;
                int cost = base - saved + knightDetour + kingWalk;
                ans = min(ans, cost);
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
