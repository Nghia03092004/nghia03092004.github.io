#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 736: Paths to Equality
 *
 * r(x,y)=(x+1,2y), s(x,y)=(2x,y+1). Find shortest odd-length path from (45,90) to x=y.
 */


int main() {
    // BFS for shortest odd-length path
    long long a = 45, b = 90;

    for (int len = 1; len <= 25; len += 2) {
        // Enumerate all 2^len paths
        long long total = 1LL << len;
        int found = 0;
        for (long long mask = 0; mask < total; mask++) {
            long long x = a, y = b;
            bool valid = true;
            for (int step = 0; step < len; step++) {
                if (mask & (1LL << step)) {
                    x = 2*x; y = y+1;  // s
                } else {
                    x = x+1; y = 2*y;  // r
                }
                if (step < len - 1 && x == y) { valid = false; break; }
            }
            if (valid && x == y) {
                printf("Length %d, final value = %lld\n", len, x);
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    return 0;
}
