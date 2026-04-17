#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 907: Minimum Path Sum in Triangle
 *
 * Triangle: T(i,j) = ((i+1)*(j+1)*37 + 13) % 100, 0 <= j <= i, N = 1000 rows.
 * Find the minimum path sum from apex to base.
 *
 * Bellman equation: V(i,j) = T(i,j) + min(V(i+1,j), V(i+1,j+1))
 *
 * Method: Bottom-up DP in O(N^2) time, O(N) space.
 *
 * The in-place update processes j = 0..i left-to-right, which is safe
 * because dp[j+1] (from the previous row) is read before being overwritten.
 */

const int N = 1000;

int main() {
    // Generate triangle and perform bottom-up DP simultaneously
    // First, store the full triangle (needed for bottom-up)
    vector<vector<int>> tri(N);
    for (int i = 0; i < N; i++) {
        tri[i].resize(i + 1);
        for (int j = 0; j <= i; j++) {
            tri[i][j] = ((long long)(i + 1) * (j + 1) * 37 + 13) % 100;
        }
    }

    // Bottom-up DP
    vector<int> dp = tri[N - 1];
    for (int i = N - 2; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            dp[j] = tri[i][j] + min(dp[j], dp[j + 1]);
        }
    }

    cout << dp[0] << endl;

    // Verification for small N
    // Build small triangle and solve both by DP and brute-force (N=10)
    int sN = 10;
    vector<vector<int>> stri(sN);
    for (int i = 0; i < sN; i++) {
        stri[i].resize(i + 1);
        for (int j = 0; j <= i; j++)
            stri[i][j] = ((long long)(i + 1) * (j + 1) * 37 + 13) % 100;
    }

    // DP for small
    vector<int> sdp = stri[sN - 1];
    for (int i = sN - 2; i >= 0; i--)
        for (int j = 0; j <= i; j++)
            sdp[j] = stri[i][j] + min(sdp[j], sdp[j + 1]);

    // Brute-force DFS for small
    int best = INT_MAX;
    function<void(int, int, int)> dfs = [&](int i, int j, int total) {
        total += stri[i][j];
        if (i == sN - 1) {
            best = min(best, total);
            return;
        }
        dfs(i + 1, j, total);
        dfs(i + 1, j + 1, total);
    };
    dfs(0, 0, 0);

    assert(sdp[0] == best);

    return 0;
}
