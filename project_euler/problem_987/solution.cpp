#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 987: Cyclotomic Polynomial Evaluation
 *
 * Compute S = sum_{n=1}^{500} Phi_n(2) mod (10^9 + 7).
 *
 * Two methods implemented:
 *   1. Divisor recurrence:  Phi_n(2) = (2^n - 1) / prod_{d|n, d<n} Phi_d(2)
 *   2. Mobius product:      Phi_n(2) = prod_{d|n} (2^d - 1)^{mu(n/d)}
 *
 * Both yield the same result; we use method 1 as primary and method 2
 * as a cross-check.
 */

const long long MOD = 1e9 + 7;
const int N = 500;

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

long long modinv(long long a, long long mod = MOD) {
    return power(a, mod - 2, mod);
}

vector<int> get_divisors(int n) {
    vector<int> divs;
    for (int d = 1; (long long)d * d <= n; d++) {
        if (n % d == 0) {
            divs.push_back(d);
            if (d != n / d) divs.push_back(n / d);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

// Method 1: Divisor recurrence
// Phi_n(2) = (2^n - 1) / prod_{d|n, d<n} Phi_d(2)
long long solve_recurrence() {
    vector<long long> phi(N + 1, 0);
    long long total = 0;

    for (int n = 1; n <= N; n++) {
        long long num = (power(2, n, MOD) - 1 + MOD) % MOD;
        long long den = 1;
        for (int d : get_divisors(n)) {
            if (d < n) {
                den = den * phi[d] % MOD;
            }
        }
        phi[n] = num % MOD * modinv(den) % MOD;
        total = (total + phi[n]) % MOD;
    }
    return total;
}

// Method 2: Mobius product formula
// Phi_n(2) = prod_{d|n} (2^d - 1)^{mu(n/d)}
long long solve_mobius() {
    // Sieve Mobius function
    vector<int> mu(N + 1, 0);
    mu[1] = 1;
    vector<bool> is_prime(N + 1, true);
    vector<int> primes;

    for (int i = 2; i <= N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if ((long long)i * p > N) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }

    // Precompute 2^d - 1 mod p
    vector<long long> pow2m1(N + 1);
    for (int d = 1; d <= N; d++) {
        pow2m1[d] = (power(2, d, MOD) - 1 + MOD) % MOD;
    }

    long long total = 0;
    for (int n = 1; n <= N; n++) {
        long long phi_n = 1;
        for (int d : get_divisors(n)) {
            int m = mu[n / d];
            if (m == 1) {
                phi_n = phi_n * pow2m1[d] % MOD;
            } else if (m == -1) {
                phi_n = phi_n * modinv(pow2m1[d]) % MOD;
            }
            // m == 0: skip (contributes 1)
        }
        total = (total + phi_n) % MOD;
    }
    return total;
}

int main() {
    long long ans1 = solve_recurrence();
    long long ans2 = solve_mobius();

    // Cross-check both methods
    assert(ans1 == ans2);

    cout << ans1 << endl;
    return 0;
}
