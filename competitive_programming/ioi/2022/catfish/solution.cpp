#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

long long max_weights(int N, int M, vector<int> X, vector<int> Y,
                       vector<int> W) {
    // For each column, collect catfish sorted by row
    vector<vector<pair<int, ll>>> fish(N); // fish[col] = {(row, weight)}
    for (int i = 0; i < M; i++)
        fish[X[i]].push_back({Y[i], W[i]});

    // Prefix sums of fish weights per column, indexed by row
    // psum[c][y] = sum of weights of fish in column c at rows 0..y-1
    vector<vector<ll>> psum(N, vector<ll>(N + 1, 0));
    for (int c = 0; c < N; c++) {
        for (auto [y, w] : fish[c])
            psum[c][y + 1] += w;
        for (int y = 1; y <= N; y++)
            psum[c][y] += psum[c][y - 1];
    }

    // sum of fish in column c at rows [lo, hi) = psum[c][hi] - psum[c][lo]

    // dp[h] = best achievable up to current column with pier height h
    // h = 0: no pier
    vector<ll> dp(N + 1, 0);

    for (int c = 0; c < N; c++) {
        vector<ll> ndp(N + 1, 0);

        // Contribution of fish in column c-1 caught by pier in column c (height h_cur):
        // Fish at (c-1, y) with y < h_cur and y >= h_prev
        // = psum[c-1][min(h_cur, N)] - psum[c-1][h_prev]  (if h_cur > h_prev)

        // Contribution of fish in column c caught by pier in column c-1 (height h_prev):
        // Fish at (c, y) with y < h_prev and y >= h_cur
        // = psum[c][min(h_prev, N)] - psum[c][h_cur]  (if h_prev > h_cur)

        // Case 1: h_prev <= h_cur
        // Left fish contribution (from column c-1): psum[c-1][h_cur] - psum[c-1][h_prev]
        // = psum[c-1][h_cur] - psum[c-1][h_prev]
        // dp[h_prev] - psum[c-1][h_prev] is what we maximize over h_prev <= h_cur
        // ndp[h_cur] = max over h_prev <= h_cur of (dp[h_prev] - psum[c-1][h_prev]) + psum[c-1][h_cur]

        // Case 2: h_prev > h_cur
        // Right fish contribution (from column c): psum[c][h_prev] - psum[c][h_cur]
        // ndp[h_cur] = max over h_prev > h_cur of (dp[h_prev] + psum[c][h_prev]) - psum[c][h_cur]

        if (c == 0) {
            // No column c-1, no contributions from left
            // dp[h_prev] is all 0
            // Only consider h_prev = 0 (no previous column)
            for (int h = 0; h <= N; h++)
                ndp[h] = 0;
        } else {
            // Case 1: scan h_cur from 0 to N, maintaining max of dp[h_prev] - psum[c-1][h_prev]
            ll best1 = LLONG_MIN;
            for (int h = 0; h <= N; h++) {
                best1 = max(best1, dp[h] - psum[c - 1][h]);
                ll val = best1 + psum[c - 1][h]; // h_cur = h
                ndp[h] = max(ndp[h], val);
            }

            // Case 2: scan h_cur from N to 0, maintaining max of dp[h_prev] + psum[c][h_prev]
            ll best2 = LLONG_MIN;
            for (int h = N; h >= 0; h--) {
                best2 = max(best2, dp[h] + psum[c][h]);
                ll val = best2 - psum[c][h]; // h_cur = h
                ndp[h] = max(ndp[h], val);
            }
        }

        dp = ndp;
    }

    ll ans = 0;
    for (int h = 0; h <= N; h++)
        ans = max(ans, dp[h]);

    return ans;
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    vector<int> X(M), Y(M), W(M);
    for (int i = 0; i < M; i++)
        scanf("%d %d %d", &X[i], &Y[i], &W[i]);
    printf("%lld\n", max_weights(N, M, X, Y, W));
    return 0;
}
