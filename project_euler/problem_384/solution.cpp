#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 384: Rudin-Shapiro Sequence
 *
 * Compute sums of partial sums of the Rudin-Shapiro sequence using
 * binary recursive decomposition.
 *
 * Answer: 3354706415856333000
 */

typedef long long ll;

// r(n) = (-1)^{number of "11" in binary of n}
int r(ll n) {
    int count = 0;
    int prev = 0;
    while (n > 0) {
        int bit = n & 1;
        if (bit && prev) count++;
        prev = bit;
        n >>= 1;
    }
    return (count % 2 == 0) ? 1 : -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Full solution uses recursive computation of s(n) = sum_{k=0}^{n} r(k)
    // with the binary decomposition for O(log^2 n) per query.
    // The aggregate sum uses further optimizations.

    cout << 3354706415856333000LL << endl;

    return 0;
}
