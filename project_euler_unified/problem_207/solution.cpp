#include <bits/stdc++.h>
using namespace std;

int main() {
    // 4^t = 2^t + k, let x = 2^t, then k = x(x-1).
    // Partitions are indexed by integer x >= 2.
    // Total partitions up to k = x(x-1): x - 1.
    // Perfect partitions: x is a power of 2.
    // Count of perfect partitions up to k = x(x-1): floor(log2(x)).
    // P(k) = floor(log2(x)) / (x-1).
    // P(k) is minimized at left endpoints k = x(x-1).
    // Find smallest x(x-1) where (x-1) > 12345 * floor(log2(x)).

    long long target = 12345;

    for (long long x = 2; x < 300000; x++) {
        int m = 63 - __builtin_clzll(x); // floor(log2(x))
        if ((x - 1) > target * m) {
            cout << x * (x - 1) << endl;
            return 0;
        }
    }

    return 0;
}
