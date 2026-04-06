#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 911: Geometric Series Truncation
 *
 * S(r, n) = sum_{k=0}^{n} r^k = (r^{n+1} - 1) / (r - 1)
 * Find sum_{r=2}^{100} S(r, r) mod 10^9+7.
 *
 * Two methods:
 *   1. Closed-form: S(r,r) = (r^{r+1} - 1) * (r-1)^{-1} mod p
 *   2. Direct summation: iterate powers 1, r, r^2, ..., r^r
 *
 * Modular inverse via Fermat: (r-1)^{p-2} mod p.
 */

const long long MOD = 1e9 + 7;

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

/* Method 1: Closed-form with Fermat inverse */
long long solve_closed() {
    long long total = 0;
    for (int r = 2; r <= 100; r++) {
        long long num = (power(r, r + 1, MOD) - 1 + MOD) % MOD;
        long long inv = power(r - 1, MOD - 2, MOD);
        total = (total + num % MOD * inv % MOD) % MOD;
    }
    return total;
}

/* Method 2: Direct summation */
long long solve_direct() {
    long long total = 0;
    for (int r = 2; r <= 100; r++) {
        long long s = 0, rk = 1;
        for (int k = 0; k <= r; k++) {
            s = (s + rk) % MOD;
            rk = rk * r % MOD;
        }
        total = (total + s) % MOD;
    }
    return total;
}

int main() {
    long long ans1 = solve_closed();
    long long ans2 = solve_direct();

    assert(ans1 == ans2);

    // Verify small exact values
    // S(2,2) = 7, S(3,3) = 40, S(4,4) = 341
    assert((power(2, 3, MOD) - 1 + MOD) % MOD * power(1, MOD - 2, MOD) % MOD == 7);
    assert((power(3, 4, MOD) - 1 + MOD) % MOD * power(2, MOD - 2, MOD) % MOD == 40);

    cout << ans1 << endl;
    return 0;
}
