#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;
const int MAXN = 10010001;

long long fac[MAXN], inv_fac[MAXN];

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
    fac[0] = 1;
    for (int i = 1; i < MAXN; i++)
        fac[i] = fac[i-1] * i % MOD;
    inv_fac[MAXN-1] = power(fac[MAXN-1], MOD-2, MOD);
    for (int i = MAXN-2; i >= 0; i--)
        inv_fac[i] = inv_fac[i+1] * (i+1) % MOD;
}

long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fac[n] % MOD * inv_fac[k] % MOD * inv_fac[n-k] % MOD;
}

int main() {
    precompute();

    // Sieve primes in (10^7, 10^7 + 10^4)
    const int LO = 10000001, HI = 10010000;
    vector<bool> is_prime(HI + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= HI; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= HI; j += i)
                is_prime[j] = false;
        }
    }

    long long ans = 0;

    for (int p = LO; p < HI; p++) {
        if (!is_prime[p]) continue;

        double a = sqrt((double)p);
        int K = (int)floor(a);  // floor(sqrt(p))

        // G(p) = sum_{k=0}^{K} C(n_k, k)
        // where n_k = p - 1 - floor(k * (a - 1))
        // This counts compositions reaching p from base region [0, a)
        long long gp = 0;
        for (int k = 0; k <= K; k++) {
            // Number of unit steps when k large jumps of size a are taken
            // The total distance covered is p from starting point in [0, a)
            // After k jumps of size a, remaining ~ p - k*a unit steps
            // The jumps can be interleaved: C(unit_steps + k - 1, k) or similar
            // More precisely: n_k = p - 1 - (int)(k * (a - 1))
            int nk = p - 1 - (int)floor(k * (a - 1.0));
            if (nk < k) break;
            gp = (gp + C(nk, k)) % MOD;
        }

        ans = (ans + gp) % MOD;
    }

    cout << ans << endl;
    return 0;
}
