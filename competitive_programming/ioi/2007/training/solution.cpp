#include <bits/stdc++.h>
using namespace std;

struct ExtraEdge {
    int u;
    int v;
    long long w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> tree_adj(N + 1);
    vector<int> eu(M), ev(M);
    vector<long long> ew(M);
    for (int i = 0; i < N - 1; ++i) {
        cin >> eu[i] >> ev[i] >> ew[i];
        tree_adj[eu[i]].push_back(ev[i]);
        tree_adj[ev[i]].push_back(eu[i]);
    }
    for (int i = N - 1; i < M; ++i) {
        cin >> eu[i] >> ev[i] >> ew[i];
    }

    vector<int> parent(N + 1, 0), depth(N + 1, 0), index_in_parent(N + 1, -1);
    vector<vector<int>> children(N + 1);
    vector<int> order;
    order.reserve(N);

    queue<int> q;
    q.push(1);
    parent[1] = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        for (int to : tree_adj[v]) {
            if (to == parent[v]) continue;
            parent[to] = v;
            depth[to] = depth[v] + 1;
            index_in_parent[to] = (int)children[v].size();
            children[v].push_back(to);
            q.push(to);
        }
    }

    int LOG = 1;
    while ((1 << LOG) <= N) ++LOG;
    vector<vector<int>> up(LOG, vector<int>(N + 1, 0));
    for (int v = 1; v <= N; ++v) up[0][v] = max(parent[v], 0);
    for (int k = 1; k < LOG; ++k) {
        for (int v = 1; v <= N; ++v) up[k][v] = up[k - 1][up[k - 1][v]];
    }

    auto lca = [&](int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = LOG - 1; k >= 0; --k) {
            if (diff & (1 << k)) a = up[k][a];
        }
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][a] != up[k][b]) {
                a = up[k][a];
                b = up[k][b];
            }
        }
        return parent[a];
    };

    long long total_extra_weight = 0;
    vector<vector<ExtraEdge>> edges_at_lca(N + 1);
    for (int i = N - 1; i < M; ++i) {
        total_extra_weight += ew[i];
        if ((depth[eu[i]] ^ depth[ev[i]]) & 1) continue;
        int w = lca(eu[i], ev[i]);
        edges_at_lca[w].push_back({eu[i], ev[i], ew[i]});
    }

    vector<vector<long long>> dp(N + 1);

    auto branch_info = [&](int root, int endpoint) {
        pair<int, long long> result{-1, 0};
        if (endpoint == root) return result;
        int cur = endpoint;
        int prev = -1;
        long long contrib = 0;
        while (cur != root) {
            if (prev == -1) {
                contrib += dp[cur][0];
            } else {
                contrib += dp[cur][1 << index_in_parent[prev]];
            }
            prev = cur;
            cur = parent[cur];
        }
        result.first = index_in_parent[prev];
        result.second = contrib;
        return result;
    };

    for (int it = (int)order.size() - 1; it >= 0; --it) {
        int v = order[it];
        int deg = (int)children[v].size();
        int states = 1 << deg;
        dp[v].assign(states, 0);

        vector<long long> removed_sum(states, 0);
        for (int mask = 1; mask < states; ++mask) {
            int bit = __builtin_ctz(mask);
            removed_sum[mask] =
                removed_sum[mask ^ (1 << bit)] + dp[children[v][bit]][0];
        }

        long long total_child_sum = removed_sum[states - 1];
        for (int mask = states - 1; mask >= 0; --mask) {
            dp[v][mask] = total_child_sum - removed_sum[mask];
        }

        struct Transition {
            int branch_mask;
            long long extra;
        };
        vector<Transition> trans;
        trans.reserve(edges_at_lca[v].size());
        for (const ExtraEdge &e : edges_at_lca[v]) {
            auto left = branch_info(v, e.u);
            auto right = branch_info(v, e.v);
            int branch_mask = 0;
            if (left.first != -1) branch_mask |= 1 << left.first;
            if (right.first != -1) branch_mask |= 1 << right.first;
            long long extra = e.w + left.second + right.second;
            trans.push_back({branch_mask, extra});
        }

        for (int mask = states - 1; mask >= 0; --mask) {
            for (const Transition &tr : trans) {
                if (mask & tr.branch_mask) continue;
                dp[v][mask] = max(dp[v][mask], tr.extra + dp[v][mask | tr.branch_mask]);
            }
        }
    }

    cout << total_extra_weight - dp[1][0] << '\n';
    return 0;
}
