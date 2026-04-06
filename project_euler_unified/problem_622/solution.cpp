#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Problem 622: Riffle Shuffles
 *
 * A perfect out-shuffle on 2n cards has order ord_{2n+1}(2).
 * Find sum of deck sizes 2n where ord_{2n+1}(2) = 60.
 *
 * Strategy: enumerate all divisors of 2^60 - 1, check which have
 * multiplicative order of 2 equal to exactly 60.
 *
 * 2^60 - 1 = 3^2 * 5^2 * 7 * 11 * 13 * 31 * 41 * 61 * 151 * 331 * 1321
 *
 * Two methods:
 *   1. Divisor enumeration with order check (primary)
 *   2. Direct order computation for small m (verification)
 */

// --- Fast modular exponentiation ---
ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// --- Check if ord_m(2) == 60 ---
// Criterion: 2^60 = 1 mod m AND 2^{60/p} != 1 mod m for p in {2,3,5}
bool has_order_60(ll m) {
    if (power(2, 60, m) != 1) return false;
    if (power(2, 30, m) == 1) return false;  // 60/2
    if (power(2, 20, m) == 1) return false;  // 60/3
    if (power(2, 12, m) == 1) return false;  // 60/5
    return true;
}

// --- Direct multiplicative order (for verification) ---
ll mult_order(ll a, ll m) {
    if (m == 1) return 1;
    ll ord = 1, cur = a % m;
    while (cur != 1) {
        cur = (lll)cur * a % m;
        ord++;
        if (ord > m) return -1;
    }
    return ord;
}

// --- Method 1: Enumerate divisors of 2^60 - 1 ---
ll solve_divisor_enum() {
    vector<pair<ll, int>> factors = {
        {3, 2}, {5, 2}, {7, 1}, {11, 1}, {13, 1},
        {31, 1}, {41, 1}, {61, 1}, {151, 1}, {331, 1}, {1321, 1}
    };
    int nf = factors.size();
    vector<int> maxexp(nf);
    for (int i = 0; i < nf; i++) maxexp[i] = factors[i].second;

    ll total = 0;
    int count = 0;

    // Iterate over all exponent combinations
    vector<int> e(nf, 0);
    while (true) {
        ll d = 1;
        for (int i = 0; i < nf; i++) {
            ll pk = 1;
            for (int j = 0; j < e[i]; j++) pk *= factors[i].first;
            d *= pk;
        }

        if (d > 1 && has_order_60(d)) {
            total += d - 1;
            count++;
        }

        // Increment exponent vector (odometer)
        int carry = 1;
        for (int i = 0; i < nf && carry; i++) {
            e[i] += carry;
            if (e[i] > maxexp[i]) {
                e[i] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        if (carry) break;
    }

    return total;
}

// --- Method 2: Brute force for small decks ---
ll solve_brute(int max_deck) {
    ll total = 0;
    for (int ds = 2; ds <= max_deck; ds += 2) {
        if (mult_order(2, ds + 1) == 60) {
            total += ds;
        }
    }
    return total;
}

int main() {
    // Method 1: Full solution
    ll ans1 = solve_divisor_enum();
    cout << "Divisor enumeration: " << ans1 << endl;

    // Method 2: Verify with small decks
    ll brute_small = solve_brute(10000);
    cout << "Brute force (decks <= 10000): " << brute_small << endl;

    // Cross-check: the brute force sum should match the enum sum
    // restricted to m <= 10001
    // (The full answer includes much larger m values)

    assert(ans1 == 3010983666182123972LL);
    cout << "\nAnswer: " << ans1 << endl;

    return 0;
}
