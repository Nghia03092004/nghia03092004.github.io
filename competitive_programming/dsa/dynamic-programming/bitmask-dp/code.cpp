long long assignment_dp(const vector<vector<long long>>& cost) {
    int n = (int)cost.size();
    const long long INF = (long long)4e18;
    vector<long long> dp(1 << n, INF);
    dp[0] = 0;

    for (int mask = 0; mask < (1 << n); ++mask) {
        int row = __builtin_popcount((unsigned)mask);
        if (row >= n) continue;
        for (int col = 0; col < n; ++col) {
            if ((mask >> col) & 1) continue;
            int next_mask = mask | (1 << col);
            dp[next_mask] = min(dp[next_mask], dp[mask] + cost[row][col]);
        }
    }

    return dp[(1 << n) - 1];
}
