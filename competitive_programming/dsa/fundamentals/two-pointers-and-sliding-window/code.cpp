int longest_window_sum_at_most(const vector<int>& a, long long limit) {
    long long sum = 0;
    int best = 0;
    for (int l = 0, r = 0; r < (int)a.size(); ++r) {
        sum += a[r];
        while (sum > limit) {
            sum -= a[l++];
        }
        best = max(best, r - l + 1);
    }
    return best;
}

long long count_subarrays_at_most_k_distinct(const vector<int>& a, int k) {
    unordered_map<int, int> freq;
    long long answer = 0;
    for (int l = 0, r = 0; r < (int)a.size(); ++r) {
        ++freq[a[r]];
        while ((int)freq.size() > k) {
            if (--freq[a[l]] == 0) {
                freq.erase(a[l]);
            }
            ++l;
        }
        answer += r - l + 1;
    }
    return answer;
}
