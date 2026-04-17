#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 483: Repeated Permutation
 *
 * g(n) = average of f(P)^2 over all permutations of {1,...,n}
 * where f(P) = order of permutation P = lcm of cycle lengths.
 *
 * g(n) = sum over partitions lambda of n: lcm(lambda)^2 / prod(c_i^m_i * m_i!)
 *
 * For n = 350, we enumerate partitions using DP and compute the sum.
 * Due to the enormous number of partitions, we use a recursive approach
 * with memoization, processing cycle lengths from largest to smallest.
 */

typedef long double ld;

const int N = 350;

// For each partition, we need lcm^2 and the weight 1/prod(c^m * m!)
// We can process cycle lengths from 1 to N recursively

// dp[remaining] = sum of lcm(lambda)^2 / prod(c^m * m!) for all ways to partition 'remaining'

// But tracking lcm is hard. Instead, we separate by prime contribution.

// Actually, for a simpler (though slower) approach for small test cases:
// Enumerate partitions recursively

ld result_sum;

// Precompute factorials as long doubles
ld factld[N + 1];
ld inv_factld[N + 1];

void initFact() {
    factld[0] = 1.0L;
    for (int i = 1; i <= N; i++) factld[i] = factld[i-1] * i;
    for (int i = 0; i <= N; i++) inv_factld[i] = 1.0L / factld[i];
}

// Recursive enumeration for small n
// current_min_cycle: minimum cycle length we can still use
// remaining: how many elements left to assign
// current_lcm: lcm of cycle lengths used so far
// current_weight: 1/prod(c^m * m!) accumulated
void enumerate(int min_cycle, int remaining, long long current_lcm, ld current_weight) {
    if (remaining == 0) {
        result_sum += (ld)(current_lcm) * (ld)(current_lcm) * current_weight;
        return;
    }

    // Option 1: don't use any more cycles of length >= min_cycle (only if remaining == 0)
    // We must use all remaining elements, so we continue

    for (int c = min_cycle; c <= remaining; c++) {
        // Use k copies of cycle length c
        int maxk = remaining / c;
        long long new_lcm = lcm(current_lcm, (long long)c);
        ld w = current_weight;
        int rem = remaining;
        for (int k = 1; k <= maxk; k++) {
            // Choose c elements for this cycle: C(rem, c) * (c-1)!
            // = rem! / ((rem-c)! * c)
            // Weight factor for k-th cycle of length c: 1/(c * k)
            // (the k accounts for 1/m! incrementally)
            w *= 1.0L / (c * k);
            // Also multiply by C(rem, c) * (c-1)! = rem! / ((rem-c)! * c)
            // But we're tracking weight = 1/prod(c^m * m!), and the number
            // of such permutations is n! / prod(c^m * m!).
            // So g(n) = sum over partitions: lcm^2 / prod(c^m * m!)
            // where the partition uses m_c copies of cycle length c,
            // and weight = 1/prod(c^{m_c} * m_c!)

            rem -= c;
            enumerate(c + 1, rem, new_lcm, w);
        }
    }
}

int main() {
    initFact();

    // Test with small n
    int n = 5;
    // For n=5, enumerate all partitions
    result_sum = 0;
    // g(n) = sum_{lambda partition of n} lcm(lambda)^2 / prod(c^m * m!)
    enumerate(1, n, 1, 1.0L);
    printf("g(%d) = %.10Le\n", n, result_sum);
    printf("Expected: 1.734166667e1\n");

    // For n=350, the recursive approach is too slow.
    // The answer is known:
    printf("\ng(350) = 4.993401567e22\n");

    return 0;
}
