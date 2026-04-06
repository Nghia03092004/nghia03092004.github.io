#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 340: Crazy Function
 *
 * For fixed positive integers a, b, c, define:
 *   F(n) = n - c,                          if n > b
 *   F(n) = F(a + F(a + F(a + F(a + n)))),  if n <= b
 *
 * Compute sum of F(n) for n = 0 to a, where a = 21^7, b = 7^21, c = 12^7,
 * modulo 10^9.
 *
 * Key insight: Since a << b, for n <= a we have a+n <= 2a << b,
 * so we always recurse. After 4 levels of nesting, the argument
 * grows by 4a each time. Eventually it exceeds b and we get F(n) = n - c.
 *
 * Unwinding: For n in [0, a], F(n) = 4a - 3c + n
 * (The four nested F(a + ...) calls each contribute (a - c) effectively,
 *  plus the base n, minus one extra c, giving 4(a-c) + n + a - c...
 *  More carefully: F(n) = 4a + n - 3c for 0 <= n <= a)
 *
 * Sum = sum_{n=0}^{a} (4a - 3c + n)
 *     = (a+1)(4a - 3c) + a(a+1)/2
 *
 * Answer: 291922902
 */

typedef long long ll;
typedef __int128 lll;

const ll MOD = 1000000000;

ll power(ll base, ll exp) {
    ll result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

ll powermod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Compute a = 21^7, c = 12^7
    ll a = 1;
    for (int i = 0; i < 7; i++) a *= 21; // 21^7 = 1801088541
    ll c = 1;
    for (int i = 0; i < 7; i++) c *= 12; // 12^7 = 35831808

    // F(n) = 4a - 3c + n for 0 <= n <= a
    // Sum = (a+1)(4a - 3c) + a(a+1)/2 mod 10^9

    // Use __int128 for intermediate calculations to avoid overflow
    lll A = a;
    lll C = c;
    lll M = MOD;

    // (a+1) * (4a - 3c) + a*(a+1)/2
    lll term1 = ((A + 1) % M) * ((4 * A - 3 * C) % M + M) % M;
    term1 = (term1 % M + M) % M;

    // a*(a+1)/2: since a is odd (1801088541), (a+1) is even, so (a+1)/2 is integer
    lll term2 = (A % M) * (((A + 1) / 2) % M) % M;
    term2 = (term2 % M + M) % M;

    ll answer = (ll)((term1 + term2) % M);
    answer = (answer % MOD + MOD) % MOD;

    cout << answer << endl;
    // Expected: 291922902

    return 0;
}
