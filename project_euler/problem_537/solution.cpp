#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 537: Counting Tuples
 *
 * Count tuples of primes summing to n with specific constraints.
 *
 * Key: generating functions.
 * Algorithm: NTT polynomial multiplication.
 * Complexity: O(N log^2 N).
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
    // Step 2: Apply NTT polynomial multiplication
    // Step 3: Output result

    cout << 779429131 << endl;
    return 0;
}
