#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// Modular exponentiation using __int128 to avoid overflow
ull mulmod(ull a, ull b, ull m) {
    return (lll)a * b % m;
}

ull powmod(ull a, ull b, ull m) {
    ull res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

// Deterministic Miller-Rabin for numbers < 3.317 * 10^24
bool is_prime(ull n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;

    // Write n-1 = d * 2^r
    ull d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    // Test with these bases (sufficient for n < 3.317 * 10^24)
    for (ull a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (a >= n) continue;
        ull x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool found = false;
        for (int i = 0; i < r - 1; i++) {
            x = mulmod(x, x, n);
            if (x == n - 1) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

int main() {
    ll LIMIT = 5000000000000000LL; // 5 * 10^15
    int count = 0;

    // n^2 + n + 1 < 5e15
    // n < sqrt(5e15) ~ 70710678
    ll nmax = (ll)sqrt((double)LIMIT) + 1;
    // Adjust nmax so that nmax^2 + nmax + 1 < LIMIT
    while (nmax * nmax + nmax + 1 >= LIMIT) nmax--;

    for (ll n = 1; n <= nmax; n++) {
        ll p = n * n + n + 1;
        if (p >= LIMIT) break;
        if (is_prime((ull)p)) {
            count++;
        }
    }

    cout << count << endl;
    return 0;
}
