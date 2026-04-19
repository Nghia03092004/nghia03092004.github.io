// IOI 2004 - Polygon
// Triangulate a convex polygon to maximize total score.
// Score of triangle (i,k,j) = w[i] * w[k] * w[j].
// Classic interval DP, O(N^3).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> w(N);
    for (int i = 0; i < N; i++) cin >> w[i];

    if (N < 3) { cout << 0 << "\n"; return 0; }

    // dp[i][j] = max score triangulating sub-polygon i..j
    vector<vector<long long>> dp(N, vector<long long>(N, 0));

    for (int len = 3; len <= N; len++) {
        for (int i = 0; i + len - 1 < N; i++) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MIN;
            for (int k = i + 1; k < j; k++) {
                long long val = dp[i][k] + dp[k][j] + w[i] * w[k] * w[j];
                dp[i][j] = max(dp[i][j], val);
            }
        }
    }

    cout << dp[0][N - 1] << "\n";
    return 0;
}
