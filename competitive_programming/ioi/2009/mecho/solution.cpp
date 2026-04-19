// IOI 2009 - Mecho
// Binary search on waiting time + multi-source BFS for bees + BFS for Mecho.
// O(N^2 log N) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, S;
    cin >> N >> S;

    vector<string> grid(N);
    int mr = 0, mc = 0, dr = 0, dc = 0;
    vector<pair<int, int>> hives;

    for (int i = 0; i < N; i++) {
        cin >> grid[i];
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 'M') { mr = i; mc = j; }
            if (grid[i][j] == 'D') { dr = i; dc = j; }
            if (grid[i][j] == 'H') hives.push_back({i, j});
        }
    }

    // Multi-source BFS: compute the time at which bees reach each cell.
    vector<vector<int>> beeTime(N, vector<int>(N, INT_MAX));
    queue<pair<int, int>> q;
    for (auto [r, c] : hives) {
        beeTime[r][c] = 0;
        q.push({r, c});
    }

    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (grid[nr][nc] == 'T' || grid[nr][nc] == 'D') continue;
            if (beeTime[nr][nc] <= beeTime[r][c] + 1) continue;
            beeTime[nr][nc] = beeTime[r][c] + 1;
            q.push({nr, nc});
        }
    }

    // Check whether Mecho can reach home if he waits t minutes before moving.
    auto canEscape = [&](int t) -> bool {
        if (beeTime[mr][mc] <= t) return false;

        vector<vector<int>> dist(N, vector<int>(N, INT_MAX));
        dist[mr][mc] = 0;
        queue<pair<int, int>> bfs;
        bfs.push({mr, mc});

        while (!bfs.empty()) {
            auto [r, c] = bfs.front(); bfs.pop();
            if (r == dr && c == dc) return true;

            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                if (grid[nr][nc] == 'T' || grid[nr][nc] == 'H') continue;
                if (dist[nr][nc] != INT_MAX) continue;

                int newDist = dist[r][c] + 1;
                int minute = t + newDist / S;
                // Mecho cannot enter a cell already reached by bees.
                if (grid[nr][nc] != 'D' && beeTime[nr][nc] <= minute) continue;

                dist[nr][nc] = newDist;
                bfs.push({nr, nc});
            }
        }
        return false;
    };

    int lo = 0, hi = N * N, ans = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (canEscape(mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    cout << ans << "\n";
    return 0;
}
