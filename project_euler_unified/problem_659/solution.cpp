#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

const ll N = 10000000;
const ll MOD18 = 1000000000000000000LL; // 10^18

// Largest prime factor of 4k^2 + 1 for each k
vector<ll> lpf;

bool is_prime_small(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

// Miller-Rabin primality test
ll mulmod(ll a, ll b, ll m) {
    return (__int128)a * b % m;
}

ll powmod(ll a, ll b, ll m) {
    ll res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

bool miller_rabin(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ll x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    // Deterministic for n < 3.3 * 10^24
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (!miller_rabin(n, a)) return false;
    }
    return true;
}

// Tonelli-Shanks: find r such that r^2 = -1 (mod p), p = 1 (mod 4)
ll sqrt_neg1(ll p) {
    // Find a non-residue
    ll g = 2;
    while (powmod(g, (p - 1) / 2, p) != p - 1) g++;
    // p - 1 = 2^s * q
    int s = 0;
    ll q = p - 1;
    while (q % 2 == 0) { q /= 2; s++; }
    // We want x^2 = -1 (mod p), i.e., x = g^((p-1)/4) when s >= 2
    return powmod(g, (p - 1) / 4, p);
}

int main() {
    // lpf[k] will store the largest prime factor of 4k^2+1
    lpf.assign(N + 1, 0);

    // Sieve: for each prime p = 1 (mod 4), find k such that p | 4k^2+1
    // 4k^2 = -1 (mod p) => (2k)^2 = -1 (mod p)
    // Let r = sqrt(-1) mod p, then 2k = r or p-r (mod p)
    // k = r/2 or (p-r)/2 (mod p)

    // We need primes up to about 4*N^2+1 ~ 4*10^14, but we can only sieve
    // small primes and handle the large prime factor separately.

    ll sieve_limit = 2 * N + 1; // ~2*10^7
    vector<bool> sieve(sieve_limit + 1, true);
    sieve[0] = sieve[1] = false;
    for (ll i = 2; i * i <= sieve_limit; i++)
        if (sieve[i])
            for (ll j = i * i; j <= sieve_limit; j += i)
                sieve[j] = false;

    // For each prime p = 1 (mod 4)
    for (ll p = 5; p <= sieve_limit; p++) {
        if (!sieve[p] || p % 4 != 1) continue;

        ll r = sqrt_neg1(p);
        // 2k = r (mod p) => k = r * inv(2) (mod p)
        ll inv2 = (p + 1) / 2; // since p is odd
        ll k1 = r * inv2 % p;
        ll k2 = (p - r) * inv2 % p;

        // Mark all k = k1 (mod p) and k = k2 (mod p) in [1, N]
        for (ll k = k1; k <= N; k += p) {
            if (k > 0) lpf[k] = max(lpf[k], p);
        }
        for (ll k = k2; k <= N; k += p) {
            if (k > 0) lpf[k] = max(lpf[k], p);
        }
    }

    // For k where 4k^2+1 might have a large prime factor > sieve_limit
    for (ll k = 1; k <= N; k++) {
        ll val = 4LL * k * k + 1;
        // Divide out all known small prime factors
        ll rem = val;
        // Quick trial division with small primes from sieve
        for (ll p = 5; p * p <= rem && p <= sieve_limit; p++) {
            if (!sieve[p] || p % 4 != 1) continue;
            while (rem % p == 0) rem /= p;
        }
        // More efficient: just check if rem > 1 and is prime
        if (rem > 1 && is_prime(rem)) {
            lpf[k] = max(lpf[k], rem);
        } else if (rem > 1) {
            // rem is composite, factor it
            // For our range, rem has at most 2 prime factors
            for (ll d = 5; d * d <= rem; d += 4) {
                if (rem % d == 0) {
                    while (rem % d == 0) rem /= d;
                    lpf[k] = max(lpf[k], d);
                }
            }
            if (rem > 1) lpf[k] = max(lpf[k], rem);
        }
    }

    // Sum up
    ull ans = 0;
    for (ll k = 1; k <= N; k++) {
        ans += (ull)lpf[k];
        ans %= (ull)MOD18;
    }

    printf("%018llu\n", ans);
    return 0;
}
