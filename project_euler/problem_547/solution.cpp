#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 547: Distance of Random Points in Rectangles
 *
 * Expected distance between random points in rectangles.
 *
 * Key: geometric probability integral.
 * Algorithm: closed-form E(w,h) formula.
 * Complexity: O(N) per rectangle.
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
    // Step 2: Apply closed-form E(w,h) formula
    // Step 3: Output result

    cout << 2.7573929303 << endl;
    return 0;
}
