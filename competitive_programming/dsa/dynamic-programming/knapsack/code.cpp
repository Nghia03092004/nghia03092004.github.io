long long knapsack_01(const vector<int>& weight, const vector<long long>& value, int capacity) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < (int)weight.size(); ++i) {
        for (int w = capacity; w >= weight[i]; --w) {
            dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
        }
    }
    return *max_element(dp.begin(), dp.end());
}

long long knapsack_unbounded(const vector<int>& weight, const vector<long long>& value, int capacity) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < (int)weight.size(); ++i) {
        for (int w = weight[i]; w <= capacity; ++w) {
            dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
        }
    }
    return *max_element(dp.begin(), dp.end());
}
