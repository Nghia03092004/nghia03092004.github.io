// IOI 2005 - Rivers (alternate formulation)
// Tree DP with knapsack. Place K sawmills to minimize transport cost.
// dp[v][j][flag]: min cost in subtree v, j sawmills, flag = sawmill at v.
// O(N * K^2) for N <= 100, K <= 50.
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int N, K;
vector<int> children[105];
int w[105], d[105];
long long dp2[105][55][2];
int sz[105];

void solve(int v, long long distToAncestor) {
    sz[v] = 1;
    for (int j = 0; j <= K; j++) {
        dp2[v][j][0] = INF;
        dp2[v][j][1] = INF;
    }
    dp2[v][0][0] = (long long)w[v] * distToAncestor;
    if (K >= 1) dp2[v][1][1] = 0;

    for (int c : children[v]) {
        // flag=0: no sawmill at v, child sees distToAncestor + d[c]
        solve(c, distToAncestor + d[c]);
        long long f0[55];
        for (int j = 0; j <= K; j++)
            f0[j] = min(dp2[c][j][0], dp2[c][j][1]);

        // flag=1: sawmill at v, child sees d[c]
        solve(c, d[c]);
        long long f1[55];
        for (int j = 0; j <= K; j++)
            f1[j] = min(dp2[c][j][0], dp2[c][j][1]);

        // Knapsack merge
        long long ndp[55][2];
        for (int j = 0; j <= K; j++) { ndp[j][0] = INF; ndp[j][1] = INF; }

        int mj1 = min(sz[v], K), mj2 = min(sz[c], K);
        for (int j1 = 0; j1 <= mj1; j1++) {
            for (int flag = 0; flag < 2; flag++) {
                if (dp2[v][j1][flag] >= INF) continue;
                for (int j2 = 0; j2 <= mj2 && j1 + j2 <= K; j2++) {
                    long long cc = (flag == 0) ? f0[j2] : f1[j2];
                    if (cc >= INF) continue;
                    ndp[j1 + j2][flag] = min(ndp[j1 + j2][flag], dp2[v][j1][flag] + cc);
                }
            }
        }

        for (int j = 0; j <= K; j++) {
            dp2[v][j][0] = ndp[j][0];
            dp2[v][j][1] = ndp[j][1];
        }
        sz[v] += sz[c];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> K;
    for (int i = 1; i <= N; i++) {
        int p;
        cin >> w[i] >> d[i] >> p;
        if (p != 0)
            children[p].push_back(i);
    }

    // Root (vertex 1) has the lumber camp (sawmill at distance 0)
    solve(1, 0);

    long long ans = INF;
    for (int j = 0; j <= K; j++)
        ans = min(ans, min(dp2[1][j][0], dp2[1][j][1]));
    cout << ans << "\n";

    return 0;
}
