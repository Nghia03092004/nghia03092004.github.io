#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 905: Lattice Point Visibility
 *
 * Count lattice points (a,b) with 1 <= a,b <= N and gcd(a,b) = 1.
 * Formula: V(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2
 *
 * Two methods:
 *   1. Direct Mobius summation: O(N) after O(N) sieve
 *   2. Hyperbola method: O(sqrt(N)) summation after sieve
 *
 * Asymptotic: V(N) ~ 6N^2/pi^2
 */

const long long MOD = 1e9 + 7;

/*
 * Linear sieve for Mobius function
 * mu[n] = 0 if n has a squared prime factor
 * mu[n] = (-1)^k if n is a product of k distinct primes
 */
vector<int> mobius_sieve(int n) {
    vector<int> mu(n + 1, 0);
    mu[1] = 1;
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if ((long long)i * p > n) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
    return mu;
}

/*
 * Method 1: Direct summation
 * V(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2 mod p
 */
long long solve_direct(int N, const vector<int>& mu) {
    long long result = 0;
    for (int d = 1; d <= N; d++) {
        if (mu[d] != 0) {
            long long q = (N / d) % MOD;
            long long term = q * q % MOD;
            if (mu[d] == 1) {
                result = (result + term) % MOD;
            } else {
                result = (result - term + MOD) % MOD;
            }
        }
    }
    return result;
}

/*
 * Method 2: Hyperbola method with prefix sums of mu
 * Group terms by the value of floor(N/d), which takes O(sqrt(N)) values.
 * Requires prefix sums of mu (Mertens function).
 */
long long solve_hyperbola(int N, const vector<int>& mu) {
    // Mertens function: M(k) = sum_{d=1}^{k} mu(d)
    vector<long long> M(N + 1, 0);
    for (int i = 1; i <= N; i++)
        M[i] = M[i - 1] + mu[i];

    long long result = 0;
    int d = 1;
    while (d <= N) {
        int q = N / d;
        int d2 = N / q;  // largest d' with floor(N/d') = q
        // Sum mu[d..d2] = M(d2) - M(d-1)
        long long mu_sum = ((M[d2] - M[d - 1]) % MOD + MOD) % MOD;
        long long qm = q % MOD;
        result = (result + mu_sum % MOD * (qm * qm % MOD)) % MOD;
        d = d2 + 1;
    }
    return result;
}

int main() {
    const int N = 1000000;

    auto mu = mobius_sieve(N);

    long long ans1 = solve_direct(N, mu);
    long long ans2 = solve_hyperbola(N, mu);

    assert(ans1 == ans2);

    // Verify small case: V(10) = 63
    auto mu10 = mobius_sieve(10);
    long long v10 = 0;
    for (int d = 1; d <= 10; d++) {
        if (mu10[d] != 0) {
            long long q = 10 / d;
            v10 += mu10[d] * q * q;
        }
    }
    assert(v10 == 63);

    cout << ans1 << endl;
    return 0;
}
