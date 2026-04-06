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

// S(k) = 10^Q * (5 + (R+1)(R+2)/2) - 9Q - R - 6
// where Q = k/9, R = k%9
long long S(long long k) {
    if (k <= 0) return 0;
    long long Q = k / 9;
    long long R = k % 9;

    long long p10 = power(10, Q, MOD);

    // coefficient: 5 + (R+1)(R+2)/2
    long long coeff = (5 + (R + 1) * (R + 2) / 2) % MOD;

    long long result = p10 % MOD * coeff % MOD;
    result = (result - 9 % MOD * (Q % MOD) % MOD + MOD) % MOD;
    result = (result - R % MOD + MOD) % MOD;
    result = (result - 6 + MOD) % MOD;

    return result;
}

int main() {
    // Compute Fibonacci numbers
    // f_90 fits in unsigned 64-bit? f_90 ~ 2.88e18, yes
    vector<long long> fib(91);
    fib[0] = 0; fib[1] = 1;
    for (int i = 2; i <= 90; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    long long ans = 0;
    for (int i = 2; i <= 90; i++) {
        ans = (ans + S(fib[i])) % MOD;
    }

    cout << ans << endl;
    return 0;
}
