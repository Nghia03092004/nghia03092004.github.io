#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 115: Counting Block Combinations II
 * Find least n where F(50, n) > 1,000,000.
 *
 * Recurrence: F(m, n) = F(m, n-1) + P(n-m-1)
 * where P(k) = sum of F(m, j) for j = -1..k
 *
 * Cross-checks:
 *   F(3, 7) = 17
 *   F(10, n) first exceeds 10^6 at n = 57
 */

int main() {
    const int m = 50;
    const long long target = 1000000;

    map<int, long long> f, prefix;
    f[-1] = 1; f[0] = 1;
    prefix[-1] = 1; prefix[0] = 2;

    for (int n = 1; ; n++) {
        long long p = 0;
        int idx = n - m - 1;
        if (idx >= -1 && prefix.count(idx)) {
            p = prefix[idx];
        }
        f[n] = f[n - 1] + p;
        prefix[n] = prefix[n - 1] + f[n];

        if (f[n] > target) {
            assert(n == 168);
            cout << n << endl;
            return 0;
        }
    }
}
