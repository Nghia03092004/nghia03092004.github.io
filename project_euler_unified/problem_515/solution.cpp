#include <bits/stdc++.h>
using namespace std;

// Digit DP to count integers in [1, N] with digit sum s in base b

vector<int> to_base(long long n, int b) {
    if (n == 0) return {0};
    vector<int> ds;
    while (n > 0) { ds.push_back(n % b); n /= b; }
    reverse(ds.begin(), ds.end());
    return ds;
}

// dp[pos][rem][tight]
map<tuple<int,int,int>, long long> memo;
vector<int> digs;
int BASE, LEN;

long long dp(int pos, int rem, int tight) {
    if (rem < 0) return 0;
    if (pos == LEN) return (rem == 0) ? 1 : 0;
    auto key = make_tuple(pos, rem, tight);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int limit = tight ? digs[pos] : BASE - 1;
    long long total = 0;
    for (int d = 0; d <= limit; d++) {
        total += dp(pos + 1, rem - d, tight && (d == limit));
    }
    return memo[key] = total;
}

long long D(int b, int s, long long N) {
    if (N <= 0 || s < 0) return 0;
    BASE = b;
    digs = to_base(N, b);
    LEN = digs.size();
    memo.clear();
    return dp(0, s, 1);
}

int main() {
    // Example: D(10, s, 10^6) for various s
    long long N = 1000000;
    int b = 10;

    long long total = 0;
    for (int s = 1; s <= 54; s++) {
        long long val = D(b, s, N);
        if (val > 0) {
            cout << "D(" << b << ", " << s << ", " << N << ") = " << val << endl;
            total += val;
        }
    }
    cout << "Total: " << total << " (should be " << N << ")" << endl;
    return 0;
}
