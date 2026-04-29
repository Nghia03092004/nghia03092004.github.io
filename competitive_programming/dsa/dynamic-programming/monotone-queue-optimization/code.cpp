vector<long long> dp_sliding_window_min(const vector<long long>& add, int k) {
    int n = (int)add.size();
    const long long INF = (long long)4e18;
    vector<long long> dp(n, INF);
    deque<int> dq;

    dp[0] = add[0];
    dq.push_back(0);

    for (int i = 1; i < n; ++i) {
        while (!dq.empty() && dq.front() < i - k) {
            dq.pop_front();
        }
        dp[i] = dp[dq.front()] + add[i];
        while (!dq.empty() && dp[dq.back()] >= dp[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
    }

    return dp;
}
