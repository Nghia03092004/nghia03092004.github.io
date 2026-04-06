#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 839: Beans in Bowls
 *
 * Count solutions to x_1 + ... + x_k = n with 0 <= x_i <= m.
 *
 * B(n,k,m) = sum_{j=0}^{floor(n/(m+1))} (-1)^j C(k,j) C(n-j(m+1)+k-1, k-1)
 *
 * Two methods: inclusion-exclusion and DP.
 */

const long long MOD = 1e9 + 7;
const int MAXFACT = 2000001;

long long fact[MAXFACT], inv_fact[MAXFACT];

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

void precompute_factorials(int n) {
    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;
    inv_fact[n] = power(fact[n], MOD - 2, MOD);
    for (int i = n - 1; i >= 0; i--)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
}

long long comb(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] % MOD * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

// Method 1: Inclusion-exclusion
long long solve_ie(int n, int k, int m) {
    long long result = 0;
    for (int j = 0; j <= k; j++) {
        long long rem = n - (long long)j * (m + 1);
        if (rem < 0) break;
        long long term = comb(k, j) * comb(rem + k - 1, k - 1) % MOD;
        if (j % 2 == 0)
            result = (result + term) % MOD;
        else
            result = (result - term + MOD) % MOD;
    }
    return result;
}

// Method 2: DP with prefix sums
long long solve_dp(int n, int k, int m) {
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;
    for (int bowl = 0; bowl < k; bowl++) {
        vector<long long> ndp(n + 1, 0);
        vector<long long> prefix(n + 2, 0);
        for (int s = 0; s <= n; s++)
            prefix[s + 1] = (prefix[s] + dp[s]) % MOD;
        for (int s = 0; s <= n; s++) {
            int lo = max(0, s - m);
            ndp[s] = (prefix[s + 1] - prefix[lo] + MOD) % MOD;
        }
        dp = ndp;
    }
    return dp[n];
}

int main() {
    precompute_factorials(MAXFACT - 1);

    // Verify on small cases
    assert(solve_ie(3, 2, 2) == 2);
    assert(solve_ie(4, 3, 2) == 6);

    // Cross-check methods on small inputs
    for (int n = 0; n <= 20; n++)
        for (int k = 1; k <= 5; k++)
            for (int m = 1; m <= 5; m++)
                assert(solve_ie(n, k, m) == solve_dp(n, k, m));

    // Solve main problem
    long long ans = solve_ie(1000, 100, 20);
    cout << ans << endl;
    return 0;
}
