#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 356: Largest Roots of Cubic Polynomials
 *
 * For g(x) = x^3 - 2^n * x^2 + n, find sum of floor(a(n)^987654321) mod 10^8
 * where a(n) is the largest real root, summed for n=1..30.
 *
 * Key insight: a(n) ~ 2^n - n/4^n, so a(n)^k is just below 2^(nk),
 * meaning floor(a(n)^k) = 2^(nk) - 1 for most n.
 * We compute 2^(nk) mod 10^8 using modular exponentiation.
 *
 * Answer: 28010159
 */

const long long MOD = 100000000LL; // 10^8

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long k = 987654321LL;
    long long ans = 0;

    for (int n = 1; n <= 30; n++) {
        // floor(a(n)^k) = 2^(n*k) - 1 mod 10^8
        long long nk = (long long)n * k;
        long long val = (power_mod(2, nk, MOD) - 1 + MOD) % MOD;
        ans = (ans + val) % MOD;
    }

    cout << ans << endl;
    return 0;
}
