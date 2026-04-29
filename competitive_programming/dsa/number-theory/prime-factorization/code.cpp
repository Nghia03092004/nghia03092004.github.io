vector<pair<long long, int>> factorize_trial(long long n) {
    vector<pair<long long, int>> factors;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p != 0) continue;
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        factors.push_back({p, exp});
    }
    if (n > 1) {
        factors.push_back({n, 1});
    }
    return factors;
}

void generate_divisors_dfs(int idx, long long cur,
                           const vector<pair<long long, int>>& factors,
                           vector<long long>& divisors) {
    if (idx == (int)factors.size()) {
        divisors.push_back(cur);
        return;
    }
    auto [p, exp] = factors[idx];
    long long value = 1;
    for (int e = 0; e <= exp; ++e) {
        generate_divisors_dfs(idx + 1, cur * value, factors, divisors);
        value *= p;
    }
}

vector<long long> generate_divisors(const vector<pair<long long, int>>& factors) {
    vector<long long> divisors;
    generate_divisors_dfs(0, 1, factors, divisors);
    sort(divisors.begin(), divisors.end());
    return divisors;
}
