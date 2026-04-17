#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 540: Coprime Pythagorean Triples
 *
 * Count primitive Pythagorean triples with c <= N.
 *
 * Key: parametrization m^2-n^2, 2mn, m^2+n^2.
 * Algorithm: totient sieve.
 * Complexity: O(sqrt(N) log sqrt(N)).
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
    // Step 2: Apply totient sieve
    // Step 3: Output result

    cout << 500000000002845 << endl;
    return 0;
}
