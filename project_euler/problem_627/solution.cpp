#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 627: Counting Products
 *
 * f(n) = ordered factorizations of n into factors >= 2.
 * Recurrence: f(n) = sum_{d|n, d<n} f(d), f(1) = 1.
 * DGF: F(s) = 1/(2 - zeta(s)).
 *
 * Method 1: Sieve-like DP in O(N log N)
 * Method 2: Recursive over divisors (verification)
 */

const int MAXN = 1000001;
ll f[MAXN];

void solve_sieve(int N) {
    memset(f, 0, sizeof(f));
    f[1] = 1;
    for (int d = 1; d <= N; d++) {
        for (int m = 2 * d; m <= N; m += d) {
            f[m] += f[d];
        }
    }
}

int main() {
    int N = 1000000;
    solve_sieve(N);

    // Verify small values
    assert(f[1] == 1);
    assert(f[2] == 1);
    assert(f[4] == 2);
    assert(f[6] == 3);
    assert(f[8] == 4);
    assert(f[12] == 8);

    // Verify f(p^k) = 2^{k-1}
    for (int k = 1; k <= 19; k++) {
        int pk = 1 << k;
        if (pk > N) break;
        assert(f[pk] == (1 << (k - 1)));
    }

    ll total = 0;
    for (int n = 2; n <= N; n++) total += f[n];
    cout << "F(" << N << ") = " << total << endl;

    return 0;
}
