#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 738: Counting Ordered Factorisations
 *
 * d(n,k) = non-decreasing factorizations of n into k parts. D(N,K) = sum.
 */


const int MOD = 1e9 + 7;

int d_count(int n, int k, int minv) {
    if (k == 1) return (n >= minv) ? 1 : 0;
    int total = 0;
    for (int d = minv; d <= n; d++) {
        if (n % d == 0)
            total += d_count(n / d, k - 1, d);
    }
    return total;
}

int main() {
    // Verify D(10, 10) and D(100, 100)
    int total = 0;
    for (int n = 1; n <= 10; n++)
        for (int k = 1; k <= 10; k++)
            total += d_count(n, k, 1);
    printf("D(10, 10) = %d\n", total);

    total = 0;
    for (int n = 1; n <= 100; n++)
        for (int k = 1; k <= 100; k++)
            total += d_count(n, k, 1);
    printf("D(100, 100) = %d\n", total);

    return 0;
}
