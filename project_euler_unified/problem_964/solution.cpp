/*
 * Problem 964: Generalized Pentagonal Numbers
 *
 * Sum all omega_k = k(3k-1)/2 for k = +-1, +-2, ... below 10^7.
 *
 * Complexity: O(sqrt(N)).
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long N = 10000000;
    long long total = 0;

    for (long long k = 1; ; k++) {
        long long v1 = k * (3*k - 1) / 2;
        long long v2 = k * (3*k + 1) / 2;
        if (v1 >= N && v2 >= N) break;
        if (v1 < N) total += v1;
        if (v2 < N) total += v2;
    }

    cout << total << endl;
    return 0;
}
