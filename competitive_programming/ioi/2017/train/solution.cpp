// IOI 2017 - Train
// Two-player Buchi game: A wants to visit charging stations infinitely often.
// Iteratively remove nodes that cannot reach a charging station,
// then compute the attractor for player B.
// Time: O(n * (n + m)), Space: O(n + m)
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<int> owner(n), charge(n);
    for (int i = 0; i < n; i++) scanf("%d", &owner[i]);
    for (int i = 0; i < n; i++) scanf("%d", &charge[i]);

    vector<vector<int>> adj(n), radj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    vector<bool> removed(n, false);

    bool changed = true;
    while (changed) {
        changed = false;

        // Step 1: BFS backward from charging stations to find reachable nodes
        vector<bool> canReach(n, false);
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (!removed[i] && charge[i]) {
                canReach[i] = true;
                q.push(i);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : radj[u]) {
                if (!removed[v] && !canReach[v]) {
                    canReach[v] = true;
                    q.push(v);
                }
            }
        }

        // Mark nodes that cannot reach any charging station as losing
        queue<int> losing;
        for (int i = 0; i < n; i++) {
            if (!removed[i] && !canReach[i]) {
                removed[i] = true;
                losing.push(i);
                changed = true;
            }
        }

        // Step 2: Compute attractor of losing nodes for player B.
        // B-controlled node: losing if ANY successor is removed.
        // A-controlled node: losing if ALL successors are removed.
        vector<int> cur_deg(n, 0);
        for (int i = 0; i < n; i++) {
            if (removed[i]) continue;
            for (int j : adj[i])
                if (!removed[j]) cur_deg[i]++;
        }

        while (!losing.empty()) {
            int u = losing.front(); losing.pop();
            for (int v : radj[u]) {
                if (removed[v]) continue;
                cur_deg[v]--;
                if (owner[v] == 1) {
                    // B can choose to go to a removed (losing) node
                    removed[v] = true;
                    losing.push(v);
                    changed = true;
                } else {
                    // A loses only if all successors are removed
                    if (cur_deg[v] == 0) {
                        removed[v] = true;
                        losing.push(v);
                        changed = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
        printf("%d\n", removed[i] ? 0 : 1);

    return 0;
}
