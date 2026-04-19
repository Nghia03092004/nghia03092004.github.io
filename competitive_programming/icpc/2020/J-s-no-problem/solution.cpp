#include <bits/stdc++.h>
using namespace std;

namespace {

const long long NEG_INF = -(1LL << 60);

struct Edge {
    int to;
    int weight;
};

void solve() {
    int n;
    cin >> n;

    vector<vector<Edge>> graph(n + 1);
    long long total = 0;
    for (int i = 0; i < n - 1; ++i) {
        int a, b, d;
        cin >> a >> b >> d;
        graph[a].push_back({b, d});
        graph[b].push_back({a, d});
        total += d;
    }

    vector<int> parent(n + 1, 0);
    vector<int> parent_weight(n + 1, 0);
    vector<int> order;
    order.reserve(n);
    order.push_back(1);
    parent[1] = -1;
    for (int idx = 0; idx < int(order.size()); ++idx) {
        int u = order[idx];
        for (const Edge& edge : graph[u]) {
            if (edge.to == parent[u]) {
                continue;
            }
            parent[edge.to] = u;
            parent_weight[edge.to] = edge.weight;
            order.push_back(edge.to);
        }
    }

    vector<array<array<long long, 2>, 5>> dp(n + 1);
    for (int u = 1; u <= n; ++u) {
        for (int odd = 0; odd <= 4; ++odd) {
            dp[u][odd][0] = dp[u][odd][1] = NEG_INF;
        }
    }

    for (int idx = n - 1; idx >= 0; --idx) {
        int u = order[idx];
        array<array<long long, 2>, 5> cur;
        for (int odd = 0; odd <= 4; ++odd) {
            cur[odd][0] = cur[odd][1] = NEG_INF;
        }
        cur[0][0] = 0;

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            if (v == parent[u]) {
                continue;
            }

            array<array<long long, 2>, 5> next;
            for (int odd = 0; odd <= 4; ++odd) {
                next[odd][0] = next[odd][1] = NEG_INF;
            }

            for (int used_u = 0; used_u <= 4; ++used_u) {
                for (int parity_u = 0; parity_u <= 1; ++parity_u) {
                    long long base = cur[used_u][parity_u];
                    if (base == NEG_INF) {
                        continue;
                    }
                    for (int used_v = 0; used_v <= 4; ++used_v) {
                        for (int parity_v = 0; parity_v <= 1; ++parity_v) {
                            long long child = dp[v][used_v][parity_v];
                            if (child == NEG_INF) {
                                continue;
                            }

                            int odd_without = used_u + used_v;
                            if (odd_without <= 4) {
                                next[odd_without][parity_u] = max(
                                    next[odd_without][parity_u],
                                    base + child
                                );
                            }

                            int odd_with = used_u + used_v
                                + (parity_u == 0 ? 1 : -1)
                                + (parity_v == 0 ? 1 : -1);
                            if (0 <= odd_with && odd_with <= 4) {
                                next[odd_with][parity_u ^ 1] = max(
                                    next[odd_with][parity_u ^ 1],
                                    base + child + edge.weight
                                );
                            }
                        }
                    }
                }
            }

            cur = next;
        }

        dp[u] = cur;
    }

    long long best_saved = 0;
    for (int odd = 0; odd <= 4; ++odd) {
        for (int parity = 0; parity <= 1; ++parity) {
            best_saved = max(best_saved, dp[1][odd][parity]);
        }
    }

    cout << 2 * total - best_saved << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
