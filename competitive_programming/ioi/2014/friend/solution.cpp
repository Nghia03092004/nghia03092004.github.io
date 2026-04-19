#include <bits/stdc++.h>
using namespace std;

int findSample(int n, int confidence[], int host[], int protocol[]) {
    // par[i] = representative of node i (union-find style)
    vector<int> par(n);
    vector<long long> w(n);
    iota(par.begin(), par.end(), 0);
    for (int i = 0; i < n; i++) w[i] = confidence[i];

    // Find representative
    // We don't need union-find with path compression, just direct parent
    // But we need find with path compression since merges chain
    function<int(int)> find = [&](int x) -> int {
        return par[x] == x ? x : par[x] = find(par[x]);
    };

    // Build adjacency for tree edges (protocol 0)
    vector<vector<int>> adj(n);

    for (int i = 1; i < n; i++) {
        int h = find(host[i]);
        int me = find(i);
        if (protocol[i] == 0) {
            // IAmYourFriend: tree edge between i and host
            adj[h].push_back(me);
            adj[me].push_back(h);
        } else if (protocol[i] == 1) {
            // MyFriendsAreYourFriends: twin of host, pick max
            if (w[me] > w[h]) {
                // me replaces h
                w[me] = max(w[me], w[h]); // just keep w[me]
                // Transfer adjacency
                for (int x : adj[h]) {
                    // Replace h with me in x's adj
                    adj[me].push_back(x);
                    for (auto &y : adj[x]) if (y == h) y = me;
                }
                adj[h].clear();
                par[h] = me;
            } else {
                // h absorbs me
                w[h] = max(w[h], w[me]);
                par[me] = h;
            }
        } else {
            // WeAreYourFriends: i is in clique with host and host's friends
            // Add w[i] to w[host] (they're complementary)
            w[h] += w[me];
            par[me] = h;
        }
    }

    // Now solve MWIS on the forest
    // Find all roots (nodes where find(x) == x)
    vector<bool> visited(n, false);
    long long ans = 0;

    // dp[x][0] = max independent set weight in subtree of x, not taking x
    // dp[x][1] = max independent set weight in subtree of x, taking x
    vector<long long> dp0(n, 0), dp1(n, 0);

    function<void(int, int)> dfs = [&](int u, int p) {
        visited[u] = true;
        dp1[u] = w[u];
        dp0[u] = 0;
        for (int v : adj[u]) {
            if (v == p || find(v) != v) continue;
            dfs(v, u);
            dp0[u] += max(dp0[v], dp1[v]);
            dp1[u] += dp0[v];
        }
    };

    for (int i = 0; i < n; i++) {
        if (find(i) == i && !visited[i]) {
            dfs(i, -1);
            ans += max(dp0[i], dp1[i]);
        }
    }

    return (int)ans;
}

int main() {
    int n;
    scanf("%d", &n);
    int confidence[n], host[n], protocol[n];
    for (int i = 0; i < n; i++) scanf("%d", &confidence[i]);
    host[0] = 0; protocol[0] = 0;
    for (int i = 1; i < n; i++) scanf("%d %d", &host[i], &protocol[i]);
    printf("%d\n", findSample(n, confidence, host, protocol));
    return 0;
}
