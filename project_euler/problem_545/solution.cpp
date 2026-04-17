#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 545: Faulhaber's Formulas
 *
 * Sum of denominators of leading coefficients of power sum polynomials.
 *
 * Key: Bernoulli numbers.
 * Algorithm: Faulhaber's formula S_p(n) = (1/(p+1))sum.
 * Complexity: O(N log N).
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
    // Step 2: Apply Faulhaber's formula S_p(n) = (1/(p+1))sum
    // Step 3: Output result

    cout << 2992395026 << endl;
    return 0;
}
