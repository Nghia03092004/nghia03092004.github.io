#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long mod_inv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

int main() {
    long long alpha = 10000000;  // 10^7
    long long n = 1000000000000LL; // 10^12

    // I(alpha, n) = sum_{j=1}^{alpha} (-1)^{j+1} * C(alpha, j) * (geometric sum)
    // where geometric sum = ((alpha-j)^{n+1} - 1) / ((alpha-j) - 1)

    long long ans = 0;
    long long binom = 1; // C(alpha, j), computed iteratively

    for (long long j = 1; j <= alpha; j++) {
        // Update binomial coefficient: C(alpha, j) = C(alpha, j-1) * (alpha - j + 1) / j
        binom = binom % MOD * ((alpha - j + 1) % MOD) % MOD;
        binom = binom % MOD * mod_inv(j % MOD, MOD) % MOD;

        long long i = alpha - j; // the base of the geometric series
        long long geo;

        if (i == 0) {
            // (0^{n+1} - 1) / (0 - 1) = 1
            geo = 1;
        } else if (i == 1) {
            // (1^{n+1} - 1) / (1 - 1) -> n + 1
            geo = (n + 1) % MOD;
        } else {
            // (i^{n+1} - 1) / (i - 1)
            long long num = (power(i % MOD, n + 1, MOD) - 1 + MOD) % MOD;
            long long den = (i % MOD - 1 + MOD) % MOD;
            geo = num % MOD * mod_inv(den, MOD) % MOD;
        }

        long long term = binom % MOD * geo % MOD;

        if (j % 2 == 1) {
            // (-1)^{j+1} = +1 for odd j
            ans = (ans + term) % MOD;
        } else {
            // (-1)^{j+1} = -1 for even j
            ans = (ans - term + MOD) % MOD;
        }
    }

    cout << ans << endl;
    return 0;
}
