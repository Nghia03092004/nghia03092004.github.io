#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll LIMIT = 1000000000LL;

// Small primes for generating admissible numbers
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
int nprimes = 10;

vector<ll> admissible;

// Generate all admissible numbers <= LIMIT
// An admissible number uses consecutive primes starting from 2, each at least once
void gen_admissible(int idx, ll val) {
    admissible.push_back(val);
    if (idx >= nprimes) return;
    ll p = primes[idx];
    ll v = val * p;
    while (v <= LIMIT) {
        gen_admissible(idx + 1, v);
        if (v > LIMIT / p) break;
        v *= p;
    }
}

// Miller-Rabin primality test
typedef unsigned long long ull;
typedef __int128 lll;

ull mulmod(ull a, ull b, ull m) {
    return (lll)a * b % m;
}

ull powmod(ull a, ull b, ull m) {
    ull res = 1; a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

bool is_prime(ull n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    ull d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }

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
    // Generate admissible numbers
    // Start with prime index 0 (must include 2)
    gen_admissible(1, 2); // val=2, next prime index=1 (prime 3)
    // But also need to allow increasing exponents of prime 2
    // Actually gen_admissible starts: idx=0 means we multiply by primes[0]=2.
    // Let me fix: start with val=1, idx=0.
    admissible.clear();
    // gen_admissible(idx, val): val already includes primes[0..idx-1].
    // Now try adding primes[idx]^k for k >= 1.
    // Start: val=1, idx=0. Must add at least 2^1.

    // Re-implement:
    function<void(int, ll)> gen = [&](int idx, ll val) {
        if (idx > 0) admissible.push_back(val); // val uses primes[0..idx-1]
        if (idx >= nprimes) return;
        ll p = primes[idx];
        ll v = val * p;
        while (v <= LIMIT) {
            gen(idx + 1, v);
            if (v > LIMIT / p) break;
            v *= p;
        }
    };

    gen(0, 1);

    // Find pseudo-Fortunate numbers
    set<ll> pf_set;
    for (ll n : admissible) {
        for (ll m = 2; ; m++) {
            if (is_prime(n + m)) {
                pf_set.insert(m);
                break;
            }
        }
    }

    ll sum = 0;
    for (ll m : pf_set) sum += m;
    cout << sum << endl;

    return 0;
}
