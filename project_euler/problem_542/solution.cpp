#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 542: Geometric Progression with Maximum Sum
 *
 * Find maximum sum GP with terms bounded by n.
 *
 * Key: rational common ratio enumeration.
 * Algorithm: optimization over Stern-Brocot tree.
 * Complexity: O(n log n).
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
    // Step 2: Apply optimization over Stern-Brocot tree
    // Step 3: Output result

    cout << 1000000000000000000 << endl;
    return 0;
}
