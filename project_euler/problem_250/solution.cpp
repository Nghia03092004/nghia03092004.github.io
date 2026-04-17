#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long MOD = 10000000000000000LL; // 10^16
    const int N = 250250;
    const int M = 250;

    vector<long long> dp(M, 0);
    dp[0] = 1;

    for (int i = 1; i <= N; i++) {
        // Compute i^i mod 250
        long long base = i % M;
        long long exp = i;
        long long r = 1;
        long long b = base;
        long long e = exp;
        while (e > 0) {
            if (e & 1) r = (r * b) % M;
            b = (b * b) % M;
            e >>= 1;
        }
        int ri = (int)r;

        // Update dp
        vector<long long> ndp(M);
        for (int j = 0; j < M; j++) {
            ndp[j] = (dp[j] + dp[((j - ri) % M + M) % M]) % MOD;
        }
        dp = ndp;
    }

    // Answer: dp[0] - 1 (exclude empty subset)
    long long answer = (dp[0] - 1 + MOD) % MOD;
    cout << answer << endl;
    return 0;
}
