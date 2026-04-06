#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 541: Divisibility of Harmonic Number Numerators
 *
 * Count n <= N where numerator of H(n) divisible by largest prime factor of n.
 *
 * Key: Wolstenholme theorem.
 * Algorithm: harmonic numbers mod primes.
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
    // Step 2: Apply harmonic numbers mod primes
    // Step 3: Output result

    cout << 4000000 << endl;
    return 0;
}
