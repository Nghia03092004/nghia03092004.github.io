#include <bits/stdc++.h>
using namespace std;

// IOI 2021 - Keys
// n rooms with keys, m corridors requiring specific keys. For each room i,
// compute p[i] = number of reachable rooms. Return which rooms achieve min p[i].
//
// Approach: BFS from each room, collecting keys and exploring corridors
// unlocked by collected keys. Repeat until no new rooms are discovered.
// (This is the correct O(n*(n+m)) approach; the full-score solution uses
// component merging for near-linear time.)

vector<int> find_reachable(vector<int> r, vector<int> u,
                           vector<int> v, vector<int> c) {
    int n = (int)r.size(), m = (int)u.size();

    // Adjacency list: corridors incident to each room
    vector<vector<int>> adj(n);
    for (int j = 0; j < m; j++) {
        adj[u[j]].push_back(j);
        adj[v[j]].push_back(j);
    }

    vector<int> p(n, 0);

    for (int start = 0; start < n; start++) {
        vector<bool> vis(n, false);
        set<int> key_set;
        queue<int> q;

        auto add_room = [&](int room) {
            if (vis[room]) return;
            vis[room] = true;
            q.push(room);
            key_set.insert(r[room]);
        };

        add_room(start);

        bool changed = true;
        while (changed) {
            changed = false;
            // BFS: explore corridors from visited rooms using collected keys
            while (!q.empty()) {
                int room = q.front();
                q.pop();
                for (int j : adj[room]) {
                    if (key_set.count(c[j])) {
                        int other = (u[j] == room) ? v[j] : u[j];
                        if (!vis[other]) {
                            add_room(other);
                            changed = true;
                        }
                    }
                }
            }
            // New keys may unlock previously blocked corridors
            if (changed) continue;
            // Re-scan all visited rooms for newly unlockable corridors
            for (int room_idx = 0; room_idx < n; room_idx++) {
                if (!vis[room_idx]) continue;
                for (int j : adj[room_idx]) {
                    if (key_set.count(c[j])) {
                        int other = (u[j] == room_idx) ? v[j] : u[j];
                        if (!vis[other]) {
                            add_room(other);
                            changed = true;
                        }
                    }
                }
            }
        }

        int cnt = 0;
        for (int i = 0; i < n; i++)
            if (vis[i]) cnt++;
        p[start] = cnt;
    }

    int min_p = *min_element(p.begin(), p.end());
    vector<int> ans(n, 0);
    for (int i = 0; i < n; i++)
        if (p[i] == min_p) ans[i] = 1;

    return ans;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<int> r(n), u(m), v(m), c(m);
    for (int i = 0; i < n; i++) scanf("%d", &r[i]);
    for (int j = 0; j < m; j++) scanf("%d %d %d", &u[j], &v[j], &c[j]);
    auto ans = find_reachable(r, u, v, c);
    for (int i = 0; i < n; i++)
        printf("%d%c", ans[i], " \n"[i == n - 1]);
    return 0;
}
