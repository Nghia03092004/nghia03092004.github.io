// IOI 2005 - Riv (Rivers) - compact version
// Tree DP with knapsack. Place K sawmills on a tree to minimize transport cost.
// dp[v][j][flag]: min cost in subtree v, j sawmills, flag = sawmill at v or not.
// Two DFS calls per child (for each parent-sawmill assumption).
// O(N * K^2) -- acceptable for N <= 100, K <= 50.
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int N, K;
int w[105], dist_par[105];
vector<int> ch[105];
long long dp[105][55][2]; // [vertex][sawmills][has_sawmill_at_v]
int sz[105];

void solve(int v, long long distToAncestor) {
    sz[v] = 1;
    for (int j = 0; j <= K; j++) {
        dp[v][j][0] = INF;
        dp[v][j][1] = INF;
    }
    dp[v][0][0] = (long long)w[v] * distToAncestor;
    if (K >= 1) dp[v][1][1] = 0;

    for (int c : ch[v]) {
        // Compute child DP for flag=0 (no sawmill at v): child sees distToAncestor + dist_par[c]
        solve(c, distToAncestor + dist_par[c]);
        long long f0[55];
        for (int j = 0; j <= K; j++)
            f0[j] = min(dp[c][j][0], dp[c][j][1]);

        // Compute child DP for flag=1 (sawmill at v): child sees dist_par[c]
        solve(c, dist_par[c]);
        long long f1[55];
        for (int j = 0; j <= K; j++)
            f1[j] = min(dp[c][j][0], dp[c][j][1]);

        // Knapsack merge
        long long ndp[55][2];
        for (int j = 0; j <= K; j++) { ndp[j][0] = INF; ndp[j][1] = INF; }

        int mj1 = min(sz[v], K);
        int mj2 = min(sz[c], K);

        for (int j1 = 0; j1 <= mj1; j1++) {
            for (int flag = 0; flag < 2; flag++) {
                if (dp[v][j1][flag] >= INF) continue;
                for (int j2 = 0; j2 <= mj2 && j1 + j2 <= K; j2++) {
                    long long cc = (flag == 0) ? f0[j2] : f1[j2];
                    if (cc >= INF) continue;
                    ndp[j1 + j2][flag] = min(ndp[j1 + j2][flag], dp[v][j1][flag] + cc);
                }
            }
        }

        for (int j = 0; j <= K; j++) {
            dp[v][j][0] = ndp[j][0];
            dp[v][j][1] = ndp[j][1];
        }
        sz[v] += sz[c];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> K;

    int root = -1;
    for (int i = 0; i < N; i++) {
        int p;
        cin >> w[i] >> dist_par[i] >> p;
        if (p == -1 || p == i)
            root = i;
        else
            ch[p].push_back(i);
    }
    if (root == -1) root = 0;

    // Root has the lumber camp (implicit sawmill at distance 0)
    solve(root, 0);

    long long ans = INF;
    for (int j = 0; j <= K; j++)
        ans = min(ans, min(dp[root][j][0], dp[root][j][1]));
    cout << ans << "\n";

    return 0;
}
