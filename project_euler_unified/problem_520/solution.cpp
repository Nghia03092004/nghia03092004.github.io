#include <bits/stdc++.h>
using namespace std;

// Digit DP for counting simbers up to N
// Parity bitmask: bit i = 1 means digit i appeared odd times

string num;
int L;
map<tuple<int,int,int,int>, long long> memo;

long long dp(int pos, int mask, int tight, int started) {
    if (pos == L) {
        return (started && mask == 0) ? 1 : 0;
    }
    auto key = make_tuple(pos, mask, tight, started);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int limit = tight ? (num[pos] - '0') : 9;
    long long total = 0;
    for (int d = 0; d <= limit; d++) {
        int nt = tight && (d == limit);
        if (!started && d == 0) {
            total += dp(pos + 1, mask, nt, 0);
        } else {
            total += dp(pos + 1, mask ^ (1 << d), nt, 1);
        }
    }
    return memo[key] = total;
}

long long count_simbers(long long N) {
    num = to_string(N);
    L = num.size();
    memo.clear();
    return dp(0, 0, 1, 0);
}

int main() {
    // Q(10^n) for n = 1..15
    for (int n = 1; n <= 15; n++) {
        long long N = 1;
        for (int i = 0; i < n; i++) N *= 10;
        long long q = count_simbers(N);
        cout << "Q(10^" << n << ") = " << q << endl;
    }
    return 0;
}
