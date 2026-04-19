// IOI 1992 - Problem 3: Islands
// Given elevation grid and water level queries, count islands (cells > water level).
// DSU approach: process cells in decreasing elevation, answer queries via map.
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, rnk;
    int components;

    DSU(int n) : parent(n, -1), rnk(n, 0), components(0) {}

    void activate(int x) {
        parent[x] = x;
        components++;
    }

    bool active(int x) { return parent[x] != -1; }

    int find(int x) {
        while (parent[x] != x) x = parent[x] = parent[parent[x]];
        return x;
    }

    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        components--;
    }
};

int main() {
    int R, C;
    scanf("%d%d", &R, &C);

    vector<vector<int>> elev(R, vector<int>(C));
    vector<pair<int,int>> cells;

    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) {
            scanf("%d", &elev[i][j]);
            cells.push_back({elev[i][j], i * C + j});
        }

    // Sort by decreasing elevation
    sort(cells.begin(), cells.end(), greater<pair<int,int>>());

    DSU dsu(R * C);
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};

    // Build map: water_level -> island_count
    map<int, int> levelIslands;
    int prevElev = cells[0].first + 1;

    for (auto& [e, idx] : cells) {
        if (e != prevElev) {
            levelIslands[prevElev - 1] = dsu.components;
            prevElev = e;
        }
        int r = idx / C, c = idx % C;
        dsu.activate(idx);
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                int nidx = nr * C + nc;
                if (dsu.active(nidx))
                    dsu.unite(idx, nidx);
            }
        }
    }
    levelIslands[prevElev - 1] = dsu.components;

    // Answer queries
    int Q;
    scanf("%d", &Q);
    while (Q--) {
        int L;
        scanf("%d", &L);
        auto it = levelIslands.upper_bound(L);
        if (it == levelIslands.begin())
            printf("0\n");
        else {
            --it;
            printf("%d\n", it->second);
        }
    }
    return 0;
}
