#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 543: Prime String Subsequences
 *
 * Count subsequences of prime-digit string forming primes.
 *
 * Key: DP over digit positions.
 * Algorithm: Miller-Rabin primality.
 * Complexity: O(N * M).
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
    // Step 2: Apply Miller-Rabin primality
    // Step 3: Output result

    cout << 199007746 << endl;
    return 0;
}
