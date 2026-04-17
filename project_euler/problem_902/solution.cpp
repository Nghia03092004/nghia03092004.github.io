#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 902: Permutation Cycles
 *
 * Count permutations of {1,...,20} whose longest cycle has length exactly
 * floor(20/2) = 10.  Answer: C(20) = a(20,10) - a(20,9) mod 10^9+7.
 *
 * Two methods:
 *   1. Cycle-building DP:  D(s,m) = sum_{k=1}^{min(s,m)} fallfact(s-1,k-1) * D(s-k,m)
 *   2. EGF polynomial:     F_m(x) = exp(sum_{k=1}^{m} x^k/k), extract n! * [x^n]
 *
 * Both use exact 128-bit integers for n <= 20 (no overflow issues).
 */

const long long MOD = 1e9 + 7;
const int N = 20;

/*
 * Method 1: Cycle-building DP (exact, __int128 for safety)
 *
 * D(0) = 1
 * D(s) = sum_{k=1}^{min(s,m)} (s-1)(s-2)...(s-k+1) * D(s-k)
 *
 * The factor (s-1)!/(s-k)! counts: choose k-1 elements to join element 1
 * in a k-cycle, then arrange them cyclically.
 */
__int128 count_dp(__int128 *dp, int m) {
    dp[0] = 1;
    for (int s = 1; s <= N; s++) {
        dp[s] = 0;
        __int128 coeff = 1;  // falling factorial (s-1)(s-2)...(s-k+1)
        for (int k = 1; k <= min(s, m); k++) {
            if (k >= 2)
                coeff *= (s - k + 1);
            dp[s] += coeff * dp[s - k];
        }
    }
    return dp[N];
}

/*
 * Method 2: EGF with rational arithmetic (using double for moderate n)
 * For exact verification of small cases.
 */
long long count_egf_mod(int n, int m, long long mod) {
    // Compute a(n, m) mod p using the EGF recurrence
    // f[j] = [x^j] F_m(x), computed via f'(x) = g'(x)*f(x)
    // where g(x) = sum_{k=1}^{m} x^k/k
    // Recurrence: j * f[j] = sum_{i=1}^{min(j,m)} i * (1/i) * f[j-i]
    //                       = sum_{i=1}^{min(j,m)} f[j-i]
    // So: f[j] = (1/j) * sum_{i=1}^{min(j,m)} f[j-i]
    // Then a(n,m) = n! * f[n]

    vector<long long> f(n + 1, 0);
    f[0] = 1;

    auto modinv = [&](long long a) -> long long {
        long long result = 1, exp = mod - 2;
        a %= mod;
        while (exp > 0) {
            if (exp & 1) result = result * a % mod;
            a = a * a % mod;
            exp >>= 1;
        }
        return result;
    };

    for (int j = 1; j <= n; j++) {
        long long s = 0;
        for (int i = 1; i <= min(j, m); i++) {
            s = (s + f[j - i]) % mod;
        }
        f[j] = s % mod * modinv(j) % mod;
    }

    // Multiply by n!
    long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact = fact * i % mod;

    return f[n] % mod * fact % mod;
}

int main() {
    int half = N / 2;  // = 10

    // Method 1: Cycle DP (exact)
    __int128 dp1[N + 1], dp2[N + 1];
    __int128 a1 = count_dp(dp1, half);
    __int128 b1 = count_dp(dp2, half - 1);
    long long ans1 = (long long)((a1 - b1) % MOD);
    if (ans1 < 0) ans1 += MOD;

    // Method 2: EGF mod p
    long long a2 = count_egf_mod(N, half, MOD);
    long long b2 = count_egf_mod(N, half - 1, MOD);
    long long ans2 = (a2 - b2 + MOD) % MOD;

    // Cross-check
    assert(ans1 == ans2);

    cout << ans1 << endl;  // 436180200
    return 0;
}
