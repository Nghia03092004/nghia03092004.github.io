#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long mod_inv(long long a, long long mod) {
    return power(a % mod + mod, mod - 2, mod);
}

int main() {
    long long k = 10000000;   // 10^7
    long long n = 1000000000000LL; // 10^12

    // S(k,n) = sum_{m=1}^{k} (-1)^{k-m+2} * C(k,m) * G(m-1, n)
    // G(i, n) = (i^{n+1} - 1) / (i - 1)

    long long ans = 0;
    long long binom = 1; // C(k, m) iteratively

    for (long long m = 1; m <= k; m++) {
        // C(k, m) = C(k, m-1) * (k - m + 1) / m
        binom = binom % MOD * ((k - m + 1) % MOD) % MOD;
        binom = binom * mod_inv(m, MOD) % MOD;

        long long i = m - 1; // base for geometric sum

        long long geo;
        if (i == 0) {
            geo = 1; // (0^{n+1} - 1)/(0 - 1) = 1
        } else if (i == 1) {
            geo = (n + 1) % MOD; // limit as i -> 1
        } else {
            long long num = (power(i, n + 1, MOD) - 1 + MOD) % MOD;
            long long den = (i - 1) % MOD;
            geo = num * mod_inv(den, MOD) % MOD;
        }

        long long term = binom * geo % MOD;

        // Sign: (-1)^{k - m + 2}
        long long sign_exp = k - m + 2;
        if (sign_exp % 2 == 0) {
            ans = (ans + term) % MOD;
        } else {
            ans = (ans - term + MOD) % MOD;
        }
    }

    cout << ans << endl;
    return 0;
}
