/*
 * Project Euler Problem 251: Cardano Triplets
 *
 * A Cardano Triplet (a, b, c) satisfies:
 *   cbrt(a + b*sqrt(c)) + cbrt(a - b*sqrt(c)) = 1
 *
 * Fundamental equation: (a+1)^2 * (8a-1) = 27 * b^2 * c
 * Parametrization: a = 3m-1 yields m^2 * (8m-3) = b^2 * c
 *
 * We write b = e*f, m = e*r with gcd(f,r) = 1, f^2 | (8m-3).
 * For each (r, f), valid e values form an arithmetic progression mod f^2.
 *
 * Answer: 18946051
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long N = 110000000LL;
    long long count = 0;

    for (long long r = 1; r * (r + 3) <= N; r++) {
        // f = 1 case
        {
            long long coeff = 3 * r + 1 + 8LL * r * r * r;
            long long offset = 3LL * r * r + 1;
            if (coeff <= N + offset) {
                long long emax = (N + offset) / coeff;
                if (emax >= 1) count += emax;
            }
        }

        // f >= 3, odd, gcd(f, r) = 1
        for (long long f = 3;; f += 2) {
            if (f * f * f > 8LL * r * N) break;
            if (__gcd(f, r) != 1) continue;
            long long f2 = f * f;

            long long val = (8LL * r) % f2;
            if (__gcd(val, f2) != 1LL) continue;

            // Compute modular inverse of val mod f2 via extended GCD
            long long inv_val;
            {
                long long a = val, b0 = f2, x0 = 1, x1 = 0;
                while (a > 0) {
                    long long q = b0 / a;
                    b0 -= q * a; swap(a, b0);
                    x1 -= q * x0; swap(x0, x1);
                }
                inv_val = ((x1 % f2) + f2) % f2;
            }
            long long e0 = (3LL * inv_val) % f2;
            if (e0 == 0) e0 = f2;

            long long m = r * e0;
            long long h = 8LL * m - 3;
            long long s = (3 * m - 1) + e0 * f + r * r * (h / f2);
            if (s <= N) {
                long long delta = f2 * (3 * r + f) + 8LL * r * r * r;
                long long kmax = (N - s) / delta;
                count += kmax + 1;
            }
        }
    }

    cout << count << endl;
    return 0;
}
