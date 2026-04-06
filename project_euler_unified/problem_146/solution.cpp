#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

ll mod_mul(ll a, ll b, ll m) {
    return (lll)a * b % m;
}

ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

bool miller_rabin(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ll x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mod_mul(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
        if (!miller_rabin(n, a)) return false;
    }
    return true;
}

int main() {
    // Precompute valid residues mod 210
    vector<int> valid;
    for (int r = 0; r < 210; r++) {
        if (r % 2 != 0) continue;
        if (r % 3 == 0) continue;
        if (r % 5 != 0) continue;
        // Check mod 7
        ll r2 = (ll)r * r;
        bool ok = true;
        for (int k : {1, 3, 7, 9, 13, 27}) {
            if ((r2 + k) % 7 == 0) { ok = false; break; }
        }
        if (ok) valid.push_back(r);
    }

    ll total = 0;
    int must_prime[] = {1, 3, 7, 9, 13, 27};
    int must_not_prime[] = {5, 11, 15, 17, 19, 21, 23, 25};

    for (int base = 0; base < 150000000; base += 210) {
        for (int r : valid) {
            ll n = base + r;
            if (n <= 0 || n >= 150000000) continue;
            ll n2 = n * n;

            bool ok = true;
            for (int k : must_prime) {
                if (!is_prime(n2 + k)) { ok = false; break; }
            }
            if (!ok) continue;

            for (int k : must_not_prime) {
                if (is_prime(n2 + k)) { ok = false; break; }
            }
            if (!ok) continue;

            total += n;
        }
    }

    cout << total << endl;
    return 0;
}
