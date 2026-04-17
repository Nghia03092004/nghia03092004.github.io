#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int N = 100000000; // 10^8

long long fact[2 * N + 1];
long long inv_fact[2 * N + 1];

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

void precompute() {
    fact[0] = 1;
    for (long long i = 1; i <= 2LL * N; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    inv_fact[2LL * N] = power(fact[2LL * N], MOD - 2, MOD);
    for (long long i = 2LL * N - 1; i >= 0; i--) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
    }
}

long long C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] % MOD * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

int main() {
    precompute();

    long long ans = 0;
    long long L_prev2 = 1; // L_1 = 1
    long long L_prev1 = 3; // L_2 = 3

    // k=2: C(2N-4, N-2) * L_2
    ans = (ans + C(2 * N - 4, N - 2) * L_prev1) % MOD;

    for (int k = 3; k <= N; k++) {
        long long L_k = (L_prev1 + L_prev2) % MOD;
        // c_k = C(2N - 2 - k, N - k)
        long long coeff = C(2 * N - 2 - k, N - k);
        ans = (ans + coeff * L_k) % MOD;
        L_prev2 = L_prev1;
        L_prev1 = L_k;
    }

    cout << ans << endl;
    return 0;
}
