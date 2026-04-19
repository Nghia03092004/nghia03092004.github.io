#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> nxt(N + 1);    // outgoing edge destination
    vector<long long> w(N + 1); // outgoing edge weight
    // Actually, functional graph: node i has one outgoing edge to nxt[i] with weight w[i]
    for (int i = 1; i <= N; i++) {
        cin >> nxt[i] >> w[i];
    }

    // Build reverse adjacency for tree parts
    // adj[u] = list of (v, weight) where v -> u is an edge
    vector<vector<pair<int, long long>>> radj(N + 1);
    for (int i = 1; i <= N; i++) {
        radj[nxt[i]].push_back({i, w[i]});
    }

    vector<int> visited(N + 1, 0); // 0=unvisited, 1=in cycle, 2=processed
    vector<bool> onCycle(N + 1, false);

    long long totalAns = 0;

    for (int start = 1; start <= N; start++) {
        if (visited[start]) continue;

        // Find the cycle in this component
        // Walk from 'start' until we revisit a node
        vector<int> path;
        unordered_map<int, int> pathIdx;
        int cur = start;
        while (pathIdx.find(cur) == pathIdx.end() && !visited[cur]) {
            pathIdx[cur] = path.size();
            path.push_back(cur);
            cur = nxt[cur];
        }

        if (visited[cur]) {
            // This component connects to an already-processed component
            // Mark all nodes in path as processed
            for (int u : path) visited[u] = 2;
            continue;
        }

        // cur is the start of the cycle within path
        int cycleStart = pathIdx[cur];
        vector<int> cycle;
        vector<long long> cycleWeights; // weight of edge from cycle[i] to cycle[i+1]
        for (int i = cycleStart; i < (int)path.size(); i++) {
            cycle.push_back(path[i]);
            onCycle[path[i]] = true;
        }

        int L = cycle.size();

        // Compute cycle edge weights
        cycleWeights.resize(L);
        for (int i = 0; i < L; i++) {
            cycleWeights[i] = w[cycle[i]]; // edge from cycle[i] to cycle[(i+1)%L]
        }

        // Mark all nodes in path as visited
        for (int u : path) visited[u] = 2;

        // For each cycle node, compute the deepest node in its hanging tree
        // BFS/DFS from each cycle node into its tree (via reverse edges, excluding cycle edges)
        vector<long long> depth(L); // max depth from each cycle node into its tree
        long long compAns = 0; // best answer within this component

        for (int ci = 0; ci < L; ci++) {
            int root = cycle[ci];

            // DFS into the tree hanging off root (reverse edges, not on cycle)
            // Compute tree diameter and max depth
            long long maxDepth = 0;
            long long treeDiam = 0;

            // BFS
            queue<pair<int, long long>> q;
            q.push({root, 0});

            while (!q.empty()) {
                auto [u, d] = q.front(); q.pop();
                maxDepth = max(maxDepth, d);

                for (auto [v, wt] : radj[u]) {
                    if (onCycle[v]) continue; // don't go back to cycle
                    if (visited[v] == 2) {
                        // Check if already processed in this tree
                        // Actually we need a separate visited for tree DFS
                        ;
                    }
                    q.push({v, d + wt});
                }
            }

            // For tree diameter, we need two BFS passes.
            // For simplicity, compute max depth from root.
            // The tree diameter will be handled by tracking two deepest paths.

            // Recompute with proper tree diameter
            // Use DFS to find diameter = longest path in the subtree
            long long d1 = 0, d2 = 0; // two longest depths

            function<long long(int)> treeDFS = [&](int u) -> long long {
                long long mx = 0;
                for (auto [v, wt] : radj[u]) {
                    if (onCycle[v]) continue;
                    visited[v] = 2;
                    long long childDepth = treeDFS(v) + wt;
                    treeDiam = max(treeDiam, mx + childDepth);
                    mx = max(mx, childDepth);
                }
                return mx;
            };

            maxDepth = treeDFS(root);
            depth[ci] = maxDepth;
            compAns = max(compAns, treeDiam);
        }

        // Now find the best path going through the cycle
        // Doubling trick: process 2L entries
        // For each pair (i, j) with 0 < j - i < L (on the doubled array):
        // value = depth[i%L] + depth[j%L] + prefix[j] - prefix[i]
        // where prefix[k] = sum of cycleWeights[0..k-1]

        // Prefix sums of cycle weights (doubled)
        vector<long long> prefix(2 * L + 1, 0);
        for (int i = 0; i < 2 * L; i++) {
            prefix[i + 1] = prefix[i] + cycleWeights[i % L];
        }

        // Sliding window maximum of (depth[i%L] - prefix[i]) for i in [j-L+1, j-1]
        deque<int> dq;
        for (int j = 0; j < 2 * L; j++) {
            // Remove elements outside window [j-L+1, j-1]
            while (!dq.empty() && dq.front() <= j - L) dq.pop_front();

            // Compute answer using front of deque (if any and if j > 0)
            if (!dq.empty()) {
                int i = dq.front();
                long long val = depth[j % L] + prefix[j] + depth[i % L] - prefix[i];
                compAns = max(compAns, val);
            }

            // Add j to deque
            long long jVal = depth[j % L] - prefix[j];
            while (!dq.empty()) {
                int back = dq.back();
                long long backVal = depth[back % L] - prefix[back];
                if (backVal <= jVal) dq.pop_back();
                else break;
            }
            dq.push_back(j);
        }

        totalAns += compAns;
    }

    cout << totalAns << "\n";
    return 0;
}
