#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 421: Prime Factors of n^15 + 1
 *
 * Find sum of s(n, 10^8) for 1 <= n <= 10^11, where
 * s(n,m) = sum of distinct prime factors of n^15+1 not exceeding m.
 *
 * Approach: Iterate over primes p <= 10^8.
 * For each p, count how many n in [1, 10^11] have p | n^15+1.
 * This requires solving n^15 ≡ -1 (mod p) and counting solutions.
 *
 * Answer: 2304215802083466198
 */

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 i128;

const ll N_LIMIT = 100000000LL;  // 10^8
const ll N_UPPER = 100000000000LL; // 10^11

vector<bool> is_prime;
vector<int> primes;

void sieve(int limit) {
    is_prime.assign(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= limit; i++)
        if (is_prime[i]) primes.push_back(i);
}

ll power_mod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (i128)result * base % mod;
        base = (i128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll gcd(ll a, ll b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

// Find a primitive root of prime p
ll primitive_root(ll p) {
    if (p == 2) return 1;
    ll phi = p - 1;
    ll temp = phi;
    vector<ll> factors;
    for (ll i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            factors.push_back(i);
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) factors.push_back(temp);

    for (ll g = 2; g < p; g++) {
        bool ok = true;
        for (ll f : factors) {
            if (power_mod(g, phi / f, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

int main() {
    sieve(N_LIMIT);

    ull total = 0;

    for (int p : primes) {
        if (p == 2) {
            // 2 | n^15+1 iff n is odd. Count of odd n in [1, N_UPPER]
            ll cnt = (N_UPPER + 1) / 2;
            total += (ull)2 * cnt;
            continue;
        }

        ll pm1 = p - 1;
        ll g30 = gcd(30LL, pm1);
        ll g15 = gcd(15LL, pm1);
        ll c = g30 - g15; // count of solutions in [0, p-1]

        if (c == 0) continue;

        // Find the actual residues
        // n^15 ≡ -1 (mod p) iff n = g^k where 15k ≡ (p-1)/2 (mod p-1)
        // g is a primitive root

        ll g = primitive_root(p);
        ll half = pm1 / 2; // index of -1 is (p-1)/2

        // Solve 15k ≡ half (mod pm1)
        // Solutions exist iff gcd(15, pm1) | half
        ll d = gcd(15LL, pm1);
        if (half % d != 0) continue;

        // Reduced: (15/d) * k ≡ half/d (mod pm1/d)
        ll a_coeff = 15 / d;
        ll b_val = half / d;
        ll mod_val = pm1 / d;

        // Find inverse of a_coeff mod mod_val
        ll inv_a = power_mod(a_coeff % mod_val, mod_val - 1 > 0 ?
            // Euler's theorem only works if gcd(a_coeff, mod_val) = 1
            // which it is since we divided by gcd(15, pm1)
            mod_val - 1 : 0, mod_val);
        // Check: if mod_val is not prime, use extended gcd
        // For simplicity and correctness, use extended gcd
        // But power_mod with Euler's totient might not work for non-prime mod_val

        // Extended GCD approach
        auto ext_gcd = [](ll a, ll b, ll &x, ll &y) -> ll {
            if (b == 0) { x = 1; y = 0; return a; }
            ll x1, y1;
            ll g = 0;
            // iterative
            ll old_a = a, old_b = b;
            ll old_x = 1, old_y = 0;
            ll curr_x = 0, curr_y = 1;
            while (b != 0) {
                ll q = a / b;
                ll temp_a = a; a = b; b = temp_a - q * b;
                ll temp_x = old_x; old_x = curr_x; curr_x = temp_x - q * curr_x;
                ll temp_y = old_y; old_y = curr_y; curr_y = temp_y - q * curr_y;
            }
            x = old_x; y = old_y;
            return a;
        };

        ll x, y;
        ll gg = ext_gcd(a_coeff, mod_val, x, y);
        // gg should be 1
        ll k0 = ((i128)x * b_val % mod_val + mod_val) % mod_val;

        // All solutions: k = k0 + t * mod_val for t = 0, 1, ..., d-1
        // The d residues are g^k for each solution k
        vector<ll> residues;
        for (ll t = 0; t < d; t++) {
            ll k = k0 + t * mod_val;
            k %= pm1;
            ll r = power_mod(g, k, p);
            residues.push_back(r);
        }
        sort(residues.begin(), residues.end());

        // Count n in [1, N_UPPER] with n ≡ r (mod p) for some r in residues
        ll full_periods = N_UPPER / p;
        ll remainder = N_UPPER % p;

        ll cnt = full_periods * c;
        for (ll r : residues) {
            if (r == 0) {
                // n=0 is not in [1, N_UPPER], but n ≡ 0 (mod p) means n = p, 2p, ...
                // n^15 + 1 ≡ 1 (mod p), so 0 is never a valid residue for n^15 ≡ -1
                continue;
            }
            if (r <= remainder) cnt++;
        }

        total += (ull)p * cnt;
    }

    cout << total << endl;
    return 0;
}
