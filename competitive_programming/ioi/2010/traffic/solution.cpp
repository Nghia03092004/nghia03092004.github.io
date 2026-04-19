// IOI 2010 - Traffic
// Find the node minimizing the max directional load when rooted there.
// O(N) time via rerooting trick.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> pop(N);
    for (int i = 0; i < N; i++) cin >> pop[i];

    vector<vector<int>> adj(N);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Root at 0; compute subtree populations via BFS.
    vector<long long> sub(N, 0);
    vector<int> parent(N, -1);
    vector<int> order;
    {
        vector<bool> visited(N, false);
        queue<int> q;
        q.push(0); visited[0] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
    }

    for (int i = 0; i < N; i++) sub[i] = pop[i];
    for (int i = N - 1; i >= 1; i--) {
        sub[parent[order[i]]] += sub[order[i]];
    }

    long long total = sub[0];

    // maxChild[u] = max subtree population among u's children.
    vector<long long> maxChild(N, 0);
    for (int i = 1; i < N; i++) {
        int u = order[i];
        maxChild[parent[u]] = max(maxChild[parent[u]], sub[u]);
    }

    // For node u rooted at u, max directional load = max(total - sub[u], maxChild[u]).
    long long bestLoad = LLONG_MAX;
    int bestNode = 0;
    for (int u = 0; u < N; u++) {
        long long load = max(total - sub[u], maxChild[u]);
        if (load < bestLoad) {
            bestLoad = load;
            bestNode = u;
        }
    }

    cout << bestNode << "\n";
    return 0;
}
