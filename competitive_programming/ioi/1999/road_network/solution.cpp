// IOI 1999 - Road Network (Tree Median)
// Given a weighted tree with N nodes, find the node minimizing the sum
// of distances to all other nodes. Uses two-pass rerooting technique.
// Complexity: O(N) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Edge case: single node
    if (N == 1) {
        cout << 1 << "\n" << 0 << "\n";
        return 0;
    }

    vector<vector<pair<int, long long>>> adj(N + 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    vector<long long> subSize(N + 1, 0), subSum(N + 1, 0), dist(N + 1, 0);
    vector<int> order, parent(N + 1, 0);
    vector<bool> visited(N + 1, false);

    // Pass 1: BFS to get traversal order, then compute subtree sizes bottom-up
    stack<int> stk;
    stk.push(1);
    visited[1] = true;
    parent[1] = 0;

    while (!stk.empty()) {
        int u = stk.top();
        stk.pop();
        order.push_back(u);
        for (auto& [v, w] : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                stk.push(v);
            }
        }
    }

    // Process in reverse order (leaves first) to compute subtree sizes and sums
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i];
        subSize[u] = 1;
        subSum[u] = 0;
        for (auto& [v, w] : adj[u]) {
            if (v != parent[u]) {
                subSize[u] += subSize[v];
                subSum[u] += subSum[v] + w * subSize[v];
            }
        }
    }

    // Pass 2: reroot to compute dist[u] = sum of distances from u to all nodes
    dist[1] = subSum[1];

    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];
        for (auto& [v, w] : adj[u]) {
            if (v != parent[u]) {
                // Moving center from u to v: nodes in v's subtree get closer,
                // all others get farther
                dist[v] = dist[u] - w * subSize[v] + w * (N - subSize[v]);
            }
        }
    }

    // Find the node with minimum total distance
    int bestNode = 1;
    long long bestDist = dist[1];
    for (int u = 1; u <= N; u++) {
        if (dist[u] < bestDist) {
            bestDist = dist[u];
            bestNode = u;
        }
    }

    cout << bestNode << "\n" << bestDist << "\n";

    return 0;
}
