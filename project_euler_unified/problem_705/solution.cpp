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
    long long n = 1e16;
    long long n_mod = n % MOD;
    long long nm1 = (n - 1) % MOD;
    long long inv2 = power(2, MOD - 2, MOD);
    long long p10 = power(10, n - 2, MOD);
    long long ans = 45 % MOD * n_mod % MOD;
    ans = ans * nm1 % MOD;
    ans = ans * inv2 % MOD;
    ans = ans * p10 % MOD;
    cout << ans << endl;
    return 0;
}
