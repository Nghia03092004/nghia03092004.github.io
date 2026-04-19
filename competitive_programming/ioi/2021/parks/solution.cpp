#include <bits/stdc++.h>
using namespace std;

// IOI 2021 - Parks
// Place roads between adjacent fountains (distance 2, axis-aligned) to form
// a spanning tree. Each road gets a bench at a unique odd-coordinate position.
// Bench assignment uses checkerboard parity to avoid collisions.

int construct_roads(vector<int> x, vector<int> y) {
    int n = (int)x.size();
    if (n == 1) {
        printf("0\n");
        return 1;
    }

    // Map positions to fountain indices
    map<pair<int, int>, int> pos_to_idx;
    for (int i = 0; i < n; i++)
        pos_to_idx[{x[i], y[i]}] = i;

    // Direction offsets: right, left, up, down
    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};

    // Build adjacency list with direction info
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < n; i++) {
        for (int d = 0; d < 4; d++) {
            int nx = x[i] + dx[d], ny = y[i] + dy[d];
            auto it = pos_to_idx.find({nx, ny});
            if (it != pos_to_idx.end())
                adj[i].push_back({it->second, d});
        }
    }

    // BFS spanning tree
    vector<bool> visited(n, false);
    queue<int> bfs;
    bfs.push(0);
    visited[0] = true;
    int visited_count = 1;

    vector<int> eu, ev, ea, eb;
    set<pair<int, int>> used_benches;

    while (!bfs.empty()) {
        int u = bfs.front();
        bfs.pop();
        for (auto [v, d] : adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            visited_count++;
            bfs.push(v);

            eu.push_back(u);
            ev.push_back(v);

            // Road midpoint
            int mx = (x[u] + x[v]) / 2;
            int my = (y[u] + y[v]) / 2;

            // Choose bench position using checkerboard parity
            int ba, bb;
            if (d == 0 || d == 1) {
                // Horizontal road: bench at (mx, my+1) or (mx, my-1)
                int parity = ((mx + my / 2) % 2 + 2) % 2;
                ba = mx;
                bb = (parity == 0) ? my + 1 : my - 1;
            } else {
                // Vertical road: bench at (mx+1, my) or (mx-1, my)
                int parity = ((mx / 2 + my) % 2 + 2) % 2;
                bb = my;
                ba = (parity == 0) ? mx + 1 : mx - 1;
            }

            // Collision fallback: try the other candidate
            if (used_benches.count({ba, bb})) {
                if (d == 0 || d == 1)
                    bb = (bb == my + 1) ? my - 1 : my + 1;
                else
                    ba = (ba == mx + 1) ? mx - 1 : mx + 1;
            }

            used_benches.insert({ba, bb});
            ea.push_back(ba);
            eb.push_back(bb);
        }
    }

    if (visited_count != n) return 0; // graph not connected

    printf("%d\n", (int)eu.size());
    for (int i = 0; i < (int)eu.size(); i++)
        printf("%d %d %d %d\n", eu[i], ev[i], ea[i], eb[i]);
    return 1;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> x(n), y(n);
    for (int i = 0; i < n; i++)
        scanf("%d %d", &x[i], &y[i]);
    int res = construct_roads(x, y);
    if (!res) printf("0\n");
    return 0;
}
