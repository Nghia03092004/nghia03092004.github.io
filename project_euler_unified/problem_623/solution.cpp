#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 623: Lambda Count
 *
 * Count permutations of {1,...,n} with exactly 2 cycles.
 * This is the unsigned Stirling number [n,2] = (n-1)! * H_{n-1}.
 *
 * Three methods:
 *   1. Direct formula: (n-1)! * sum(k^{-1}, k=1..n-1) mod p
 *   2. Recurrence: [n,2] = (n-1)*[n-1,2] + (n-2)!
 *   3. Inverse recurrence: inv[k] = -(p/k) * inv[p%k] mod p
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

// Method 1: Direct formula with Fermat inverses
ll solve_direct(int n) {
    if (n < 2) return 0;
    ll fact = 1, h = 0;
    for (int k = 1; k < n; k++) {
        fact = fact * k % MOD;
        h = (h + power(k, MOD - 2, MOD)) % MOD;
    }
    return fact * h % MOD;
}

// Method 2: Recurrence [n,2] = (n-1)*[n-1,2] + (n-2)!
ll solve_recurrence(int n) {
    if (n < 2) return 0;
    ll stir = 1;       // [2,2] = 1
    ll fact_nm2 = 1;   // (n-2)! for n=2 => 0! = 1
    for (int m = 3; m <= n; m++) {
        fact_nm2 = fact_nm2 * (m - 2) % MOD;
        stir = ((ll)(m - 1) * stir % MOD + fact_nm2) % MOD;
    }
    return stir;
}

// Method 3: Fast inverse recurrence
ll solve_fast(int n) {
    if (n < 2) return 0;
    vector<ll> inv(n);
    inv[1] = 1;
    for (int k = 2; k < n; k++) {
        inv[k] = -(ll)(MOD / k) % MOD * inv[MOD % k] % MOD;
        if (inv[k] < 0) inv[k] += MOD;
    }
    ll fact = 1, h = 0;
    for (int k = 1; k < n; k++) {
        fact = fact * k % MOD;
        h = (h + inv[k]) % MOD;
    }
    return fact * h % MOD;
}

int main() {
    // Verify small values
    int expected[] = {0, 0, 1, 3, 11, 50, 274, 1764, 13068, 109584, 1026576};
    for (int n = 0; n <= 10; n++) {
        ll v1 = solve_direct(n);
        ll v2 = solve_recurrence(n);
        ll v3 = (n >= 2) ? solve_fast(n) : 0;
        assert(v1 == expected[n] % MOD);
        assert(v2 == expected[n] % MOD);
        assert(v3 == expected[n] % MOD);
    }

    // Cross-check all three methods for larger n
    for (int n = 2; n <= 1000; n++) {
        ll v1 = solve_direct(n);
        ll v2 = solve_recurrence(n);
        ll v3 = solve_fast(n);
        assert(v1 == v2 && v2 == v3);
    }

    int n = 1000000;
    ll ans = solve_fast(n);
    cout << "[" << n << ", 2] mod " << MOD << " = " << ans << endl;

    return 0;
}
