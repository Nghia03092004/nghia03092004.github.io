#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 928: Catalan Number Variants
 *
 * Find sum_{k=0}^{1000} C_k mod 10^9+7.
 * C_n = (2n)! / ((n+1)! * n!).
 *
 * Recurrence: C_{k+1} = C_k * 2(2k+1) / (k+2).
 * Or use factorial precomputation with modular inverses.
 *
 * Generating function: C(x) = (1 - sqrt(1-4x)) / (2x).
 * Asymptotics: C_n ~ 4^n / (n^{3/2} sqrt(pi)).
 *
 * Complexity: O(N) with factorial precomputation.
 */

long long MOD = 1000000007;

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
    int N = 1000;
    vector<long long> fact(2*N + 2), inv_fact(2*N + 2);
    fact[0] = 1;
    for (int i = 1; i <= 2*N + 1; i++)
        fact[i] = fact[i-1] * i % MOD;
    inv_fact[2*N+1] = power(fact[2*N+1], MOD-2, MOD);
    for (int i = 2*N; i >= 0; i--)
        inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;

    long long total = 0;
    for (int k = 0; k <= N; k++) {
        // C_k = (2k)! / (k! * (k+1)!)
        long long c = fact[2*k] % MOD * inv_fact[k] % MOD * inv_fact[k+1] % MOD;
        total = (total + c) % MOD;
    }
    cout << total << endl;

    // Verify first Catalan numbers: 1, 1, 2, 5, 14, 42
    auto catalan = [&](int k) {
        return fact[2*k] % MOD * inv_fact[k] % MOD * inv_fact[k+1] % MOD;
    };
    assert(catalan(0) == 1);
    assert(catalan(1) == 1);
    assert(catalan(2) == 2);
    assert(catalan(3) == 5);
    assert(catalan(4) == 14);

    return 0;
}
