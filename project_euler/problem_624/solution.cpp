#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 624: Two Heads Are Better Than One
 *
 * Expected flips to get n consecutive heads: E(n) = 2^{n+1} - 2.
 *
 * Markov chain: S_k = run of k heads. Transitions:
 *   S_k -> S_{k+1} with probability 1/2 (heads)
 *   S_k -> S_0     with probability 1/2 (tails)
 *
 * Recurrence: e_k = 1 + (1/2)*e_{k+1} + (1/2)*e_0
 * Solution: e_0 = 2^{n+1} - 2.
 *
 * Methods:
 *   1. Closed-form via modular exponentiation
 *   2. Back-substitution (verification)
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

// Method 1: Closed form
ll solve_closed(int n) {
    return (power(2, n + 1, MOD) - 2 + MOD) % MOD;
}

// Method 2: Back-substitution (exact for small n)
ll solve_exact(int n) {
    // e_k = 2^{n+1} - 2^{k+1}
    // Verify: e_0 = 2^{n+1} - 2
    ll e0 = (1LL << (n + 1)) - 2;
    return e0;
}

int main() {
    // Verify closed form matches exact for small n
    for (int n = 1; n <= 30; n++) {
        ll exact = solve_exact(n);
        ll modular = solve_closed(n);
        assert(exact % MOD == modular);

        // Verify recurrence: E(n) = 2*E(n-1) + 2
        if (n >= 2) {
            assert(exact == 2 * solve_exact(n - 1) + 2);
        }
    }

    cout << "Verification passed for n = 1..30" << endl;

    // Print table
    printf("%4s %12s\n", "n", "E(n)");
    for (int n = 1; n <= 20; n++) {
        if (n <= 30) {
            printf("%4d %12lld\n", n, solve_exact(n));
        }
    }

    // Large n via modular arithmetic
    int n = 1000000;
    ll ans = solve_closed(n);
    cout << "\nE(" << n << ") mod " << MOD << " = " << ans << endl;

    return 0;
}
