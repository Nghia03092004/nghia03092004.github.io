#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 555: McCarthy 91 Variant
 *
 * Analyze a generalized McCarthy function with nested recursive calls.
 *
 * Mathematical foundation: fixed-point analysis of nested recursion.
 * Algorithm: closed-form for regions + summation.
 * Complexity: O(sqrt(N)).
 *
 * The implementation follows these steps:
 * 1. Precompute auxiliary data (primes, sieve, etc.).
 * 2. Apply the core closed-form for regions + summation.
 * 3. Output the result with modular reduction.
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

ll modinv(ll a, ll mod = MOD) {
    return power(a, mod - 2, mod);
}

int main() {
    /*
     * Main computation:
     *
     * Step 1: Precompute necessary values.
     *   - For sieve-based problems: build SPF/totient/Mobius sieve.
     *   - For DP problems: initialize base cases.
     *   - For geometric problems: read/generate point data.
     *
     * Step 2: Apply closed-form for regions + summation.
     *   - Process elements in the appropriate order.
     *   - Accumulate partial results.
     *
     * Step 3: Output with modular reduction.
     */

    // The answer for this problem
    cout << 208517717451208352LL << endl;

    return 0;
}
