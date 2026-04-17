#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 365: A Huge Binomial Coefficient
 *
 * Compute C(10^18, 10^9) mod p for primes p in (1000, 5000) using Lucas' theorem.
 * Then sum C(10^18, 10^9) mod (p1*p2*p3) over all triples using CRT.
 *
 * Answer: 162619462356610313
 */

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// Sieve primes in range (lo, hi)
vector<int> sieve_primes(int lo, int hi) {
    vector<bool> is_prime(hi + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= hi; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= hi; j += i)
                is_prime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = lo + 1; i < hi; i++) {
        if (is_prime[i]) primes.push_back(i);
    }
    return primes;
}

// Modular exponentiation
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

// C(n, k) mod p for n, k < p (direct computation)
ll small_comb(ll n, ll k, ll p) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    ll num = 1, den = 1;
    for (ll i = 0; i < k; i++) {
        num = num * ((n - i) % p) % p;
        den = den * ((i + 1) % p) % p;
    }
    return num % p * power(den, p - 2, p) % p;
}

// Lucas' theorem: C(n, k) mod p
ll lucas(ll n, ll k, ll p) {
    ll result = 1;
    while (n > 0 || k > 0) {
        ll ni = n % p, ki = k % p;
        if (ki > ni) return 0;
        result = result * small_comb(ni, ki, p) % p;
        n /= p;
        k /= p;
    }
    return result;
}

// Extended GCD
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// CRT for three residues
ll crt3(ll r1, ll p1, ll r2, ll p2, ll r3, ll p3) {
    lll M = (lll)p1 * p2 * p3;
    lll M1 = (lll)p2 * p3;
    lll M2 = (lll)p1 * p3;
    lll M3 = (lll)p1 * p2;

    ll x, y;
    extgcd((ll)(M1 % p1), p1, x, y);
    lll inv1 = ((lll)x % p1 + p1) % p1;

    extgcd((ll)(M2 % p2), p2, x, y);
    lll inv2 = ((lll)x % p2 + p2) % p2;

    extgcd((ll)(M3 % p3), p3, x, y);
    lll inv3 = ((lll)x % p3 + p3) % p3;

    lll result = (r1 * M1 % M * inv1 % M + r2 * M2 % M * inv2 % M + r3 * M3 % M * inv3 % M) % M;
    return (ll)result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll N = 1000000000000000000LL; // 10^18
    ll K = 1000000000LL;          // 10^9

    vector<int> primes = sieve_primes(1000, 5000);
    int sz = primes.size();

    // Compute C(N, K) mod p for each prime
    vector<ll> residues(sz);
    for (int i = 0; i < sz; i++) {
        residues[i] = lucas(N, K, primes[i]);
    }

    // Sum over all triples
    lll total = 0;
    for (int i = 0; i < sz; i++) {
        for (int j = i + 1; j < sz; j++) {
            for (int k = j + 1; k < sz; k++) {
                ll val = crt3(residues[i], primes[i],
                              residues[j], primes[j],
                              residues[k], primes[k]);
                total += val;
            }
        }
    }

    cout << (ll)total << endl;
    // Expected: 162619462356610313

    return 0;
}
