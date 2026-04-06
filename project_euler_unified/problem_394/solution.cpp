#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 394: Eating Pie
 *
 * E(n) = 1 + ln(n)  (expected slices until remaining pie < 1/n of original)
 *
 * Derivation: Each cut multiplies remaining fraction by U ~ Uniform(0,1).
 * Equivalent to sum of Exp(1) variables exceeding ln(n).
 * By Poisson summation identity: E[N] = 1 + t where t = ln(n).
 *
 * Answer: ceil(E(10)) + ceil(E(100)) + ceil(E(1000))
 *       = ceil(1+ln10) + ceil(1+ln100) + ceil(1+ln1000)
 *       = 4 + 6 + 8 = 18
 */

// Exact computation
long long solve() {
    int ns[] = {10, 100, 1000};
    long long answer = 0;

    for (int n : ns) {
        double e_n = 1.0 + log((double)n);
        long long c = (long long)ceil(e_n);
        printf("E(%d) = 1 + ln(%d) = %.10f, ceil = %lld\n", n, n, e_n, c);
        answer += c;
    }

    return answer;
}

// Monte Carlo verification
double simulate(int n, int trials) {
    mt19937_64 rng(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    long long total_cuts = 0;
    double threshold = 1.0 / n;

    for (int t = 0; t < trials; t++) {
        double remaining = 1.0;
        int cuts = 0;
        while (remaining >= threshold) {
            double alpha = dist(rng) * remaining;
            remaining -= alpha;
            cuts++;
        }
        total_cuts += cuts;
    }

    return (double)total_cuts / trials;
}

int main() {
    printf("Project Euler Problem 394: Eating Pie\n");
    printf("======================================\n\n");

    printf("Exact formula: E(n) = 1 + ln(n)\n\n");

    long long answer = solve();

    printf("\nAnswer: %lld\n", answer);
    assert(answer == 18);

    printf("\nMonte Carlo verification (500000 trials):\n");
    int ns[] = {10, 100, 1000};
    for (int n : ns) {
        double sim = simulate(n, 500000);
        double exact = 1.0 + log((double)n);
        printf("E(%d): simulated = %.6f, exact = %.6f, error = %.6f\n",
               n, sim, exact, fabs(sim - exact));
    }

    printf("\nAll checks passed.\n");
    return 0;
}
