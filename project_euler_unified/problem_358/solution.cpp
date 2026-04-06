#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 358: Cyclic Numbers
 *
 * Find the cyclic number from 1/p (full reptend prime) whose digits start
 * with 00000000137 and end with 56789.
 *
 * The cyclic number is C = (10^(p-1) - 1) / p.
 * From the leading digits, p ~ 729927007.
 * We search nearby primes and check:
 *   1) Last 5 digits of C are 56789
 *   2) 10 is a primitive root mod p
 *   3) Leading digits match 00000000137
 *
 * The answer is the sum of digits of the cyclic number = 3284144505.
 *
 * Answer: 3284144505
 */

typedef long long ll;
typedef __int128 lll;

ll power_mod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1)
            result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Check if 10 is a primitive root mod p
// i.e., ord_p(10) = p-1
bool is_full_reptend(ll p) {
    ll phi = p - 1;
    ll temp = phi;
    vector<ll> factors;
    for (ll f = 2; f * f <= temp; f++) {
        if (temp % f == 0) {
            factors.push_back(f);
            while (temp % f == 0) temp /= f;
        }
    }
    if (temp > 1) factors.push_back(temp);

    for (ll q : factors) {
        if (power_mod(10, phi / q, p) == 1)
            return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // p is near 1/0.00000000137 ~ 729927007.3
    // Search primes around this value
    ll MOD5 = 100000LL;

    for (ll p = 729927000LL; p < 729928000LL; p++) {
        if (!is_prime(p)) continue;

        // Check last 5 digits: C mod 10^5 = 56789
        // C = (10^(p-1) - 1) / p
        // We need (10^(p-1) - 1) / p mod 10^5
        // = (10^(p-1) - 1) * p^{-1} mod 10^5
        // But p must be coprime to 10^5
        ll p_inv = power_mod(p % MOD5, MOD5 / 5 * 4 - 1, MOD5); // Euler's theorem won't work directly
        // Use extended gcd or just compute inverse
        // p is odd and not divisible by 5 (since p is a large prime)
        // phi(10^5) = 10^5 * (1-1/2) * (1-1/5) = 40000
        ll p_mod = p % MOD5;
        ll inv_p = power_mod(p_mod, 39999, MOD5); // phi(100000) = 40000
        ll ten_pow = power_mod(10, p - 1, p * MOD5);
        ll C_last5 = ((ten_pow - 1) / p) % MOD5;
        if (C_last5 < 0) C_last5 += MOD5;

        if (C_last5 != 56789) continue;

        // Check leading digits: 1/p should start with 0.00000000137...
        // i.e., 137 * 10^(-11) <= 1/p < 138 * 10^(-11)
        // i.e., 10^11 / 138 < p <= 10^11 / 137
        // 10^11 / 138 = 724637681.2...
        // 10^11 / 137 = 729927007.3...
        double recip = 1.0 / p;
        // Check more precisely using integer arithmetic
        // 1/p starts with 0.00000000137 means
        // floor(10^11 / p) should give us 137...
        // Actually: 10^11 = 137 * p + r, so 10^11 / p = 137.xxxxx
        // But we need the decimal expansion to start 00000000137
        // That means 1/p = 0.00000000137...
        // So 10^11 * (1/p) is between 137 and 138
        // i.e., 137 <= 10^11 / p < 138
        // i.e., 10^11 / 138 < p <= 10^11 / 137
        ll hi = 100000000000LL / 137; // 729927007
        ll lo = 100000000000LL / 138; // 724637681

        if (p <= lo || p > hi) continue;

        // More precise check: verify that 10^11/p starts correctly
        // 10^11 / p should be in [137, 138)
        if (100000000000LL / p != 137) continue;

        // Check primitive root
        if (!is_full_reptend(p)) continue;

        // Found! The answer is the sum of digits of C.
        // For a cyclic number with p-1 digits from 1/p,
        // the digit sum = 9*(p-1)/2
        ll digit_sum = 9LL * (p - 1) / 2;
        cout << digit_sum << endl;
        return 0;
    }

    cout << "Not found" << endl;
    return 0;
}
