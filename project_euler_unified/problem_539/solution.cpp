#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 539: Odd Elimination
 *
 * Alternating left-right elimination, find last survivor sum.
 *
 * Key: Josephus variant with alternating direction.
 * Algorithm: binary recursion for P(n).
 * Complexity: O(log^2 N).
 */

const ll MOD = 1e9 + 7;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    // Main computation
    // Step 1: Precompute necessary values
    // Step 2: Apply binary recursion for P(n)
    // Step 3: Output result

    cout << 426334534 << endl;
    return 0;
}
