#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 398: Cutting Rope
 *
 * E(n, m) = expected length of 2nd shortest segment when cutting
 * a rope of length n at m-1 randomly chosen integer points.
 *
 * E[2nd min] = sum_{t>=1} P(2nd min >= t)
 * P(2nd min >= t) = P(all >= t) + P(exactly one < t)
 *
 * Both terms reduce to ratios of binomial coefficients.
 * We use log-gamma for numerical computation.
 *
 * Answer: E(10^7, 100) = 2010.59096
 */

// log of binomial coefficient C(a, b) using long double for precision
long double log_comb(long long a, long long b) {
    if (b < 0 || a < b) return -1e4000L;  // -infinity
    if (b == 0 || a == b) return 0.0L;
    return lgammal((long double)(a + 1)) - lgammal((long double)(b + 1))
         - lgammal((long double)(a - b + 1));
}

long double solve(long long n, long long m) {
    long double log_total = log_comb(n - 1, m - 1);
    long double result = 0.0L;
    long long t_max = (n - 1) / (m - 1) + 1;

    for (long long t = 1; t <= t_max; t++) {
        // P(all parts >= t)
        long long rem_all = n - m * (t - 1);
        long double p_all = 0.0L;
        if (rem_all >= m) {
            p_all = expl(log_comb(rem_all - 1, m - 1) - log_total);
        }

        // P(exactly one part < t)
        long long R = n - (m - 1) * t;
        long long S = min(t - 1, R);
        long double p_one = 0.0L;
        if (S >= 1 && R >= 1) {
            long double log_upper = log_comb(R + m - 2, m - 1) - log_total;
            long double log_lower = log_comb(R - S + m - 2, m - 1) - log_total;
            p_one = (long double)m * (expl(log_upper) - expl(log_lower));
        }

        long double prob = p_all + p_one;
        if (prob < 1e-20L && t > 2) break;
        result += prob;
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);

    // Verify examples
    printf("E(3, 2) = %.10Lf (expected 2.0)\n", solve(3, 2));
    printf("E(8, 3) = %.10Lf (expected %.10f)\n", solve(8, 3), 16.0 / 7.0);

    // Solve the main problem
    auto start = chrono::high_resolution_clock::now();
    long double answer = solve(10000000LL, 100LL);
    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end - start).count();

    printf("E(10^7, 100) = %.5Lf\n", answer);
    printf("Computation time: %.3f seconds\n", elapsed);

    return 0;
}
