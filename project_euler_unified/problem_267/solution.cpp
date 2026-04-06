#include <bits/stdc++.h>
using namespace std;

int main() {
    // PE 267: You toss a biased coin 1000 times.
    // Each toss: heads with prob 1/2, tails with prob 1/2.
    // Before EACH toss, you bet a fixed fraction f of current wealth.
    // If heads: gain 2*f*wealth (so wealth *= (1+2f))
    // If tails: lose f*wealth (so wealth *= (1-f))
    // Start with $1. Find max probability (over f) of ending with >= $10^9.
    //
    // n = 1000 flips. After k heads: wealth = (1+2f)^k * (1-f)^(1000-k)
    // Need (1+2f)^k * (1-f)^(1000-k) >= 10^9
    // k*ln(1+2f) + (1000-k)*ln(1-f) >= 9*ln(10)
    // k >= [9*ln(10) - 1000*ln(1-f)] / [ln(1+2f) - ln(1-f)]
    //
    // For each f, k_min(f) = ceil([9*ln(10) - 1000*ln(1-f)] / [ln(1+2f) - ln(1-f)])
    // Probability = P(Bin(1000, 0.5) >= k_min(f))
    // Maximize over f.
    //
    // k_min is a step function of f. We want the smallest possible k_min.
    // The continuous function g(f) = [9*ln(10) - 1000*ln(1-f)] / [ln(1+2f) - ln(1-f)]
    // Minimize g(f) over f in (0, 1), then k_min = ceil(min g(f)).

    int n = 1000;
    double target = 9.0 * log(10.0);

    // Find the f that minimizes g(f)
    // g(f) = (target - n*ln(1-f)) / (ln(1+2f) - ln(1-f))
    // = (target + n*|ln(1-f)|) / (ln(1+2f) + |ln(1-f)|)
    // As f -> 0: g(f) -> target / (2f + f) ~ target/(3f) -> infinity
    // As f -> 1: g(f) -> (target + inf) / (ln3 + inf) -> n = 1000
    // So there's a minimum somewhere in (0,1).

    // Use golden section search or ternary search
    double lo = 1e-10, hi = 1.0 - 1e-10;
    for (int iter = 0; iter < 200; iter++) {
        double m1 = lo + (hi - lo) / 3;
        double m2 = hi - (hi - lo) / 3;
        auto g = [&](double f) {
            return (target - n * log(1.0 - f)) / (log(1.0 + 2.0 * f) - log(1.0 - f));
        };
        if (g(m1) < g(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    double f_opt = (lo + hi) / 2;

    auto g_func = [&](double f) {
        return (target - n * log(1.0 - f)) / (log(1.0 + 2.0 * f) - log(1.0 - f));
    };

    double g_min = g_func(f_opt);
    int kmin = (int)ceil(g_min - 1e-9);

    // Check kmin and kmin+1 to be safe (ceiling might be off)
    // For each candidate kmin, compute probability and take the best
    long double best_prob = 0;

    for (int km = max(0, kmin - 1); km <= min(n, kmin + 1); km++) {
        // Check if there exists f such that wealth >= 10^9 with km heads
        // (1+2f)^km * (1-f)^(n-km) >= 10^9
        // At optimal f = (3*km - n)/(2*n):
        if (3 * km <= n) continue; // f <= 0
        double fstar = (3.0 * km - n) / (2.0 * n);
        if (fstar <= 0 || fstar >= 1) continue;
        double log_wealth = km * log(1.0 + 2.0 * fstar) + (n - km) * log(1.0 - fstar);
        if (log_wealth < target - 1e-9) continue; // can't reach 10^9 with km heads

        // Compute P(Bin(n, 0.5) >= km)
        long double prob = 0;
        for (int k = km; k <= n; k++) {
            long double lt = lgammal(n + 1) - lgammal(k + 1) - lgammal(n - k + 1)
                             - (long double)n * logl(2.0L);
            prob += expl(lt);
        }
        if (prob > best_prob) best_prob = prob;
    }

    printf("%.12Lf\n", best_prob);
    return 0;
}
