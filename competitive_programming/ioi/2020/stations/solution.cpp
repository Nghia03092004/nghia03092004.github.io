#include <bits/stdc++.h>
using namespace std;

// IOI 2020 - Stations
// Assign labels [0, n-1] to tree nodes such that routing can be determined
// from (current_label, target_label, sorted_neighbor_labels) alone.
// Strategy: DFS-based labeling with depth parity.
//   Even depth: label = pre-order time (before children).
//   Odd depth:  label = post-order time (after children).
// This ensures the label is the min (even) or max (odd) in its subtree,
// making parent identification and subtree range deduction possible.

vector<int> label(int n, int k, vector<int> u, vector<int> v) {
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; i++) {
        adj[u[i]].push_back(v[i]);
        adj[v[i]].push_back(u[i]);
    }

    vector<int> labels(n);
    int timer = 0;

    // Single DFS assigning labels in [0, n-1]
    function<void(int, int, int)> dfs = [&](int node, int parent, int d) {
        if (d % 2 == 0) {
            // Even depth: assign label before visiting children (pre-order)
            labels[node] = timer++;
            for (int child : adj[node])
                if (child != parent)
                    dfs(child, node, d + 1);
        } else {
            // Odd depth: assign label after visiting children (post-order)
            for (int child : adj[node])
                if (child != parent)
                    dfs(child, node, d + 1);
            labels[node] = timer++;
        }
    };
    dfs(0, -1, 0);

    return labels;
}

int find_next_station(int s, int t, vector<int> c) {
    if ((int)c.size() == 1) return c[0]; // leaf: only one choice

    if (s < c[0]) {
        // Even-depth node (pre-order label): s < all neighbors.
        // Parent = max neighbor (its post-order time encompasses entire subtree).
        // Children are c[0]..c[size-2], each with subtree range:
        //   child c[0]: labels in [s+1, c[0]]
        //   child c[i]: labels in [c[i-1]+1, c[i]]
        for (int i = 0; i < (int)c.size() - 1; i++) {
            int lo = (i == 0) ? s + 1 : c[i - 1] + 1;
            int hi = c[i];
            if (t >= lo && t <= hi) return c[i];
        }
        return c.back(); // target outside all children: route to parent
    } else {
        // Odd-depth node (post-order label): s > all neighbors.
        // Parent = min neighbor (its pre-order time precedes this subtree).
        // Children are c[1]..c[size-1], each with subtree range:
        //   child c[i]: labels in [c[i], c[i+1]-1]  (last child: [c[last], s-1])
        for (int i = 1; i < (int)c.size(); i++) {
            int lo = c[i];
            int hi = (i + 1 < (int)c.size()) ? c[i + 1] - 1 : s - 1;
            if (t >= lo && t <= hi) return c[i];
        }
        return c[0]; // target outside all children: route to parent
    }
}

// Local testing main
int main() {
    int n, k_val;
    scanf("%d %d", &n, &k_val);
    vector<int> u(n - 1), v(n - 1);
    for (int i = 0; i < n - 1; i++)
        scanf("%d %d", &u[i], &v[i]);
    auto labels = label(n, k_val, u, v);
    printf("Labels:");
    for (int i = 0; i < n; i++) printf(" %d", labels[i]);
    printf("\n");

    int q;
    scanf("%d", &q);
    while (q--) {
        int s, t;
        scanf("%d %d", &s, &t);
        int nc;
        scanf("%d", &nc);
        vector<int> c(nc);
        for (int i = 0; i < nc; i++) scanf("%d", &c[i]);
        printf("%d\n", find_next_station(s, t, c));
    }
    return 0;
}
