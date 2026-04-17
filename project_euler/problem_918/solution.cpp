#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 918: Farey Sequence Properties
 *
 * |F_n| = 1 + sum_{k=1}^{n} phi(k)
 *
 * Two methods:
 *   1. Totient sieve: compute phi(k) for k=1..n, then sum
 *   2. Mediant-based enumeration (verification)
 *
 * Asymptotic: |F_n| ~ 3n^2/pi^2
 * Mediant property: consecutive a/b, c/d satisfy bc - ad = 1
 */

const int N = 1000;

/* Euler's totient sieve */
vector<int> totient_sieve(int n) {
    vector<int> phi(n + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= n; p++) {
        if (phi[p] == p) {  // p is prime
            for (int m = p; m <= n; m += p) {
                phi[m] -= phi[m] / p;
            }
        }
    }
    return phi;
}

/* Mediant-based Farey sequence generation */
long long farey_count_mediant(int n) {
    long long count = 0;
    int a = 0, b = 1, c = 1, d = n;
    count++;  // 0/1
    while (c <= n) {
        int k = (n + b) / d;
        int na = c, nb = d, nc = k * c - a, nd = k * d - b;
        a = na; b = nb; c = nc; d = nd;
        count++;
    }
    return count;
}

int main() {
    auto phi = totient_sieve(N);

    // Method 1: Sieve
    long long ans = 1;
    for (int k = 1; k <= N; k++)
        ans += phi[k];

    // Method 2: Mediant enumeration
    long long ans2 = farey_count_mediant(N);
    assert(ans == ans2);

    // Verify small cases
    assert(1 + phi[1] == 2);  // |F_1| = 2
    long long f3 = 1;
    for (int k = 1; k <= 3; k++) f3 += phi[k];
    assert(f3 == 5);  // |F_3| = 5

    cout << ans << endl;
    return 0;
}
