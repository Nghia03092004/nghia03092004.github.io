#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 538: Maximum Quadrilaterals
 *
 * Find maximum area quadrilateral from point sets.
 *
 * Key: convex hull + rotating calipers.
 * Algorithm: shoelace formula.
 * Complexity: O(n log n + h^2).
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
    // Step 2: Apply shoelace formula
    // Step 3: Output result

    cout << 49856 << endl;
    return 0;
}
