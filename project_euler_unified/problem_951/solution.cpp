/*
 * Problem 951: Euler Product Approximation
 *
 * Compute floor(E(3, 10^6) * 10^15) where
 *   E(3, N) = prod_{p <= N, p prime} (1 - p^{-3})^{-1}
 *
 * Algorithm:
 *   1. Sieve primes up to N = 10^6
 *   2. Accumulate L = sum -ln(1 - p^{-3}) in log-space (long double)
 *   3. E = exp(L), answer = floor(E * 10^15)
 *
 * The partial Euler product converges to zeta(3) = 1.20205690315959...
 * (Apery's constant) with error O(1/(N^2 ln N)).
 *
 * Complexity: O(N log log N) for sieve, O(pi(N)) for product.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 1000000;

    // Sieve of Eratosthenes
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Accumulate Euler product in log-space using long double
    long double log_prod = 0.0L;
    int prime_count = 0;
    for (int p = 2; p <= N; p++) {
        if (is_prime[p]) {
            long double term = -logl(1.0L - powl((long double)p, -3.0L));
            log_prod += term;
            prime_count++;
        }
    }

    long double E = expl(log_prod);

    // Verification: zeta(3) = 1.2020569031595942854...
    long double zeta3 = 1.2020569031595942854L;
    long double error = zeta3 - E;

    cout << fixed << setprecision(18);
    cout << "E(3, " << N << ") = " << (double)E << endl;
    cout << "zeta(3)     = " << (double)zeta3 << endl;
    cout << "Error       = " << (double)error << endl;
    cout << "Primes used = " << prime_count << endl;
    cout << endl;

    // Final answer
    long long answer = (long long)(E * 1e15L);
    cout << answer << endl;

    return 0;
}
