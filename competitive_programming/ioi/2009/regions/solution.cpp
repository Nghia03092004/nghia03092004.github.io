// IOI 2009 - Regions
// Sqrt decomposition on region sizes. Large regions precomputed via DFS,
// small regions answered via binary search on Euler-tour order.
// O(N sqrt(N)) precomputation, O(sqrt(N)) per query.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, R, Q;
    cin >> N >> R >> Q;

    vector<int> region(N + 1);
    vector<vector<int>> children(N + 1);
    vector<vector<int>> regionNodes(R + 1);

    cin >> region[1];
    regionNodes[region[1]].push_back(1);

    for (int i = 2; i <= N; i++) {
        int p;
        cin >> p >> region[i];
        children[p].push_back(i);
        regionNodes[region[i]].push_back(i);
    }

    // Euler tour (iterative DFS).
    vector<int> tin(N + 1), tout(N + 1);
    int timer = 0;
    vector<int> euler;
    {
        stack<pair<int, bool>> stk;
        stk.push({1, false});
        while (!stk.empty()) {
            auto [u, leaving] = stk.top(); stk.pop();
            if (leaving) {
                tout[u] = timer - 1;
                continue;
            }
            tin[u] = timer++;
            euler.push_back(u);
            stk.push({u, true});
            for (int i = (int)children[u].size() - 1; i >= 0; i--) {
                stk.push({children[u][i], false});
            }
        }
    }

    int B = max(1, (int)sqrt(N));

    // Sort each region's node list by entry time.
    for (int r = 1; r <= R; r++) {
        sort(regionNodes[r].begin(), regionNodes[r].end(),
             [&](int a, int b) { return tin[a] < tin[b]; });
    }

    // Identify large regions (size >= B).
    vector<int> largeId(R + 1, -1);
    vector<int> largeRegions;
    for (int r = 1; r <= R; r++) {
        if ((int)regionNodes[r].size() >= B) {
            largeId[r] = (int)largeRegions.size();
            largeRegions.push_back(r);
        }
    }
    int nLarge = (int)largeRegions.size();

    // --- Precompute for large r1 (as ancestor region) ---
    // ansLargeR1[li][r2] = number of (e1, e2) pairs where e1 in r1 is ancestor of e2 in r2.
    vector<vector<long long>> ansLargeR1(nLarge, vector<long long>(R + 1, 0));
    for (int li = 0; li < nLarge; li++) {
        int r1 = largeRegions[li];
        struct Frame { int u; bool entered; };
        stack<Frame> dfs;
        dfs.push({1, false});
        int curCount = 0; // number of ancestors in r1 on the current root-to-node path
        while (!dfs.empty()) {
            auto &f = dfs.top();
            if (!f.entered) {
                f.entered = true;
                if (region[f.u] == r1) curCount++;
                // Strict ancestor: subtract 1 if node itself is in r1.
                int ancestorsR1 = curCount - (region[f.u] == r1 ? 1 : 0);
                ansLargeR1[li][region[f.u]] += ancestorsR1;
                for (int i = (int)children[f.u].size() - 1; i >= 0; i--) {
                    dfs.push({children[f.u][i], false});
                }
            } else {
                if (region[f.u] == r1) curCount--;
                dfs.pop();
            }
        }
    }

    // --- Precompute for large r2 (as descendant region) ---
    // ansLargeR2[r1][li] = number of pairs where r1-node is ancestor of r2-node.
    vector<vector<long long>> ansLargeR2(R + 1, vector<long long>(nLarge, 0));
    for (int li = 0; li < nLarge; li++) {
        int r2 = largeRegions[li];
        // Compute subtreeR2[u] = number of r2-nodes in subtree of u.
        // Process euler in reverse (children before parents in pre-order reversed).
        vector<int> subtreeR2(N + 1, 0);
        for (int i = (int)euler.size() - 1; i >= 0; i--) {
            int u = euler[i];
            subtreeR2[u] = (region[u] == r2) ? 1 : 0;
            for (int c : children[u]) {
                subtreeR2[u] += subtreeR2[c];
            }
        }
        // For each node u in region r1, it contributes subtreeR2[u] descendants in r2
        // (minus 1 if u itself is in r2, but each node belongs to exactly one region).
        for (int u = 1; u <= N; u++) {
            int cnt = subtreeR2[u] - (region[u] == r2 ? 1 : 0);
            if (cnt > 0) {
                ansLargeR2[region[u]][li] += cnt;
            }
        }
    }

    // Helper: count nodes of region r whose tin is in [lo, hi].
    auto countInRange = [&](int r, int lo, int hi) -> int {
        auto &v = regionNodes[r];
        auto getLo = lower_bound(v.begin(), v.end(), lo,
            [&](int node, int val) { return tin[node] < val; });
        auto getHi = upper_bound(v.begin(), v.end(), hi,
            [&](int val, int node) { return val < tin[node]; });
        return (int)(getHi - getLo);
    };

    // Answer queries.
    while (Q--) {
        int r1, r2;
        cin >> r1 >> r2;

        long long ans = 0;
        if (largeId[r1] >= 0) {
            ans = ansLargeR1[largeId[r1]][r2];
        } else if (largeId[r2] >= 0) {
            ans = ansLargeR2[r1][largeId[r2]];
        } else {
            // Both small: for each node in r1, count descendants in r2.
            for (int u : regionNodes[r1]) {
                ans += countInRange(r2, tin[u] + 1, tout[u]);
            }
        }

        cout << ans << "\n";
        cout.flush();
    }

    return 0;
}
