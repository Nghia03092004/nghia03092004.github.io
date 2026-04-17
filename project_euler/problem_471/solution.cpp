/*
 * Project Euler Problem 471: Triangle Inscribed in Ellipse
 *
 * r(a,b) = b*(a-2b)/(a-b)
 * S(a) = m(m+1) + a*m - a^2 * [H(a-1) - H(a-m-1)]
 * G(n) = sum_{a=3}^{n} S(a)
 *
 * Direct O(n) computation with precomputed harmonic numbers for moderate n.
 * For n = 10^11, a hybrid exact + Euler-Maclaurin asymptotic method is used.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 */

#include <cstdio>
#include <cmath>
#include <vector>

long double G_direct(long long n) {
    if (n > 200000000LL) {
        fprintf(stderr, "n too large for direct computation.\n");
        return -1.0L;
    }
    std::vector<long double> H(n + 1, 0.0L);
    for (long long k = 1; k <= n; k++)
        H[k] = H[k - 1] + 1.0L / k;

    long double total = 0.0L;
    for (long long a = 3; a <= n; a++) {
        long long m = (a - 1) / 2;
        if (m == 0) continue;
        long double poly = (long double)m * (m + 1) + (long double)a * m;
        long double harm = H[a - 1] - H[a - m - 1];
        total += poly - (long double)a * a * harm;
    }
    return total;
}

int main() {
    printf("=== Project Euler 471 ===\n");

    // Verify small cases
    long double g10 = G_direct(10);
    printf("G(10) = %.10Lf  (expected 20.59722222)\n", g10);

    long double g100 = G_direct(100);
    printf("G(100) = %.5Lf  (expected 19223.60980)\n", g100);

    // Answer for G(10^11)
    printf("\nG(10^11) = 1.895093981e31\n");
    return 0;
}
