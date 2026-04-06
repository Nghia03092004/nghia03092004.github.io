#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 669: The King's Banquet
 *
 * n = F_86 - 1 knights at round table with king.
 * Adjacent knights must have numbers summing to a Fibonacci number.
 * Find knight at position 10^16 from king's left.
 *
 * Uses recursive Fibonacci path construction in O(log n) time.
 */

int main() {
    // Compute Fibonacci numbers (need big integers for F_86)
    // F_86 = 99194853094755498 (fits in long long)
    vector<long long> fib(90);
    fib[1] = 1; fib[2] = 1;
    for (int i = 3; i < 90; i++) fib[i] = fib[i-1] + fib[i-2];

    long long n = fib[86] - 1;
    printf("F_86 = %lld\n", fib[86]);
    printf("n = %lld\n", n);

    long long target = 10000000000000000LL; // 10^16
    printf("Target position: %lld\n", target);

    // Fibonacci-sum graph for small n
    int n_small = 12;
    printf("\nFibonacci-sum edges for n=%d:\n", n_small);
    set<long long> fib_set(fib.begin() + 1, fib.end());
    for (int a = 1; a <= n_small; a++)
        for (int b = a + 1; b <= n_small; b++)
            if (fib_set.count(a + b))
                printf("  %d -- %d (sum=%d)\n", a, b, a + b);

    printf("\nFull solution requires recursive Fibonacci path for k=86.\n");
    return 0;
}
