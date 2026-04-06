/*
 * Project Euler Problem 457: A Polynomial Modulo the Square of a Prime
 *
 * f(n) = n^2 - 3n - 1
 * R(p) = smallest positive n with f(n) = 0 (mod p^2), or 0 if none.
 * SR(L) = sum of R(p) for primes p <= L.
 * Find SR(10^7).
 *
 * Approach:
 *   - Sieve primes up to L.
 *   - For each prime p (skip p=2, p=13), check if 13 is a QR mod p.
 *   - If yes, find sqrt(13) mod p via Tonelli-Shanks.
 *   - Compute roots of n^2 - 3n - 1 = 0 mod p using quadratic formula.
 *   - Lift roots to mod p^2 via Hensel's lemma.
 *   - Take smallest positive root as R(p) and accumulate.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 */

#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <cstdint>
#include <chrono>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

const int LIMIT = 10000000;

// Sieve of Eratosthenes
vector<int> sieve_primes(int lim) {
    vector<bool> is_prime(lim + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (ll)i * i <= lim; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= lim; j += i)
                is_prime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= lim; i++)
        if (is_prime[i]) primes.push_back(i);
    return primes;
}

// Modular exponentiation
ll powmod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Modular inverse using Fermat's little theorem (p prime)
ll modinv(ll a, ll p) {
    return powmod(a, p - 2, p);
}

// Modular inverse mod p^2 using extended approach
ll modinv_p2(ll a, ll p2) {
    // Use powmod with Euler's totient: phi(p^2) = p^2 - p
    // a^(-1) = a^(p^2 - p - 1) mod p^2
    // But simpler: use __int128 based extended gcd or just powmod
    // Since p2 might not be prime, use extended gcd
    ll g, x, y;
    // Extended GCD
    ll a0 = a % p2;
    if (a0 < 0) a0 += p2;
    ll old_r = a0, new_r = p2;
    ll old_s = 1, new_s = 0;
    while (new_r != 0) {
        ll q = old_r / new_r;
        ll tmp;
        tmp = old_r - q * new_r; old_r = new_r; new_r = tmp;
        tmp = old_s - q * new_s; old_s = new_s; new_s = tmp;
    }
    ll inv = old_s % p2;
    if (inv < 0) inv += p2;
    return inv;
}

// Tonelli-Shanks: find sqrt(n) mod p. Returns -1 if not a QR.
ll tonelli_shanks(ll n, ll p) {
    n %= p;
    if (n < 0) n += p;
    if (n == 0) return 0;
    if (powmod(n, (p - 1) / 2, p) != 1) return -1;

    if (p % 4 == 3) {
        return powmod(n, (p + 1) / 4, p);
    }

    ll q = p - 1, s = 0;
    while (q % 2 == 0) { q /= 2; s++; }

    ll z = 2;
    while (powmod(z, (p - 1) / 2, p) != p - 1) z++;

    ll m = s;
    ll c = powmod(z, q, p);
    ll t = powmod(n, q, p);
    ll r = powmod(n, (q + 1) / 2, p);

    while (true) {
        if (t == 1) return r;
        ll i = 1;
        ll temp = (lll)t * t % p;
        while (temp != 1) {
            temp = (lll)temp * temp % p;
            i++;
        }
        ll b = c;
        for (ll j = 0; j < m - i - 1; j++)
            b = (lll)b * b % p;
        m = i;
        c = (lll)b * b % p;
        t = (lll)t * c % p;
        r = (lll)r * b % p;
    }
}

int main() {
    auto t0 = chrono::high_resolution_clock::now();

    vector<int> primes = sieve_primes(LIMIT);
    printf("Sieved %zu primes up to %d\n", primes.size(), LIMIT);

    ull total = 0;
    int count_contributing = 0;

    for (int idx = 0; idx < (int)primes.size(); idx++) {
        ll p = primes[idx];
        if (p == 2 || p == 13) continue;

        ll sq = tonelli_shanks(13, p);
        if (sq < 0) continue;

        ll p2 = p * p;
        ll inv2 = modinv(2, p);

        ll r1 = (3 + sq) % p * inv2 % p;
        ll r2 = ((3 - sq % p + p) % p) * inv2 % p;

        ll best = p2;

        for (int ri = 0; ri < 2; ri++) {
            ll r = (ri == 0) ? r1 : r2;

            // f(r) mod p2
            ll fr = ((lll)r * r % p2 - 3 * r % p2 + p2 - 1) % p2;
            if (fr < 0) fr += p2;
            fr = (((lll)r * r - 3 * r - 1) % p2 + p2) % p2;

            ll fpr = ((2 * r - 3) % p + p) % p;

            if (fpr == 0) {
                // Degenerate: try all lifts
                for (ll k = 0; k < p; k++) {
                    ll rr = r + k * p;
                    ll val = (((lll)rr * rr - 3 * rr - 1) % p2 + p2) % p2;
                    if (val == 0) {
                        if (rr == 0) rr = p2;
                        if (rr < best) best = rr;
                    }
                }
                continue;
            }

            // Hensel lift
            ll fpr_p2 = ((2 * r - 3) % p2 + p2) % p2;
            ll inv_fpr = modinv_p2(fpr_p2, p2);
            ll lifted = (r - (lll)fr * inv_fpr % p2 + p2) % p2;
            // More careful computation
            lifted = ((ll)((lll)r - (lll)fr % p2 * inv_fpr % p2) % p2 + p2) % p2;
            // Even more careful:
            lll val = (lll)fr * inv_fpr;
            ll sub = (ll)(val % p2);
            lifted = ((r - sub) % p2 + p2) % p2;
            if (lifted == 0) lifted = p2;
            if (lifted < best) best = lifted;
        }

        if (best < p2) {
            total += best;
            count_contributing++;
        }
    }

    auto t1 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t1 - t0).count();

    printf("Contributing primes: %d\n", count_contributing);
    printf("SR(%d) = %llu\n", LIMIT, total);
    printf("Time: %.2f s\n", elapsed);

    return 0;
}
