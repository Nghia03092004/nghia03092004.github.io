#include <bits/stdc++.h>
using namespace std;
const long long MOD = 1e9 + 7;
long long power(long long a, long long b, long long mod) {
    long long res = 1; a %= mod;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}
int main() {
    long long N = 10000000;
    long long inv2 = power(2, MOD - 2, MOD);
    long long inv3 = power(3, MOD - 2, MOD);
    long long two_N_3 = 2 * N % MOD * inv3 % MOD;
    long long neg_half = (MOD - inv2) % MOD;
    long long neg_half_N = power(neg_half, N, MOD);
    long long geo = (MOD - 1 + MOD) % MOD * ((1 - neg_half_N + MOD) % MOD) % MOD * inv3 % MOD;
    long long ans = (two_N_3 + inv3 % MOD * geo % MOD) % MOD;
    cout << ans << endl;
    return 0;
}
