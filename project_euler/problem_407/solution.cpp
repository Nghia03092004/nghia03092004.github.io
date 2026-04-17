#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 407: Idempotents
 *
 * Find sum of M(n) for n = 1..10^7 where M(n) is the largest a < n
 * with a^2 = a (mod n).
 *
 * a(a-1) = 0 mod n. Since gcd(a,a-1) = 1, for each prime power p^e || n,
 * either p^e | a or p^e | (a-1). Enumerate via CRT.
 */

const int MAXN = 10000001;
int spf[MAXN]; // smallest prime factor

long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// CRT: find x = r1 mod m1, x = r2 mod m2
// Returns (combined_r, combined_m)
pair<long long, long long> crt(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    long long g = extgcd(m1, m2, x, y);
    long long lcm = m1 / g * m2;
    long long diff = r2 - r1;
    long long r = (r1 + m1 * ((diff / g % (m2 / g) * x) % (m2 / g))) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

int main() {
    ios_base::sync_with_stdio(false);

    // Sieve smallest prime factor
    for (int i = 2; i < MAXN; i++) spf[i] = i;
    for (int i = 2; (long long)i * i < MAXN; i++) {
        if (spf[i] == i) { // i is prime
            for (int j = i * i; j < MAXN; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    long long total = 0;

    for (int n = 2; n < MAXN; n++) {
        // Factorize n
        vector<int> primes_powers; // prime power factors
        int tmp = n;
        while (tmp > 1) {
            int p = spf[tmp];
            int pe = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pe *= p;
            }
            primes_powers.push_back(pe);
        }

        int k = primes_powers.size();
        long long best = 1;

        // Enumerate all 2^k combinations
        for (int mask = 0; mask < (1 << k); mask++) {
            // For each prime power: if bit is 0, a = 0 mod pe; if bit is 1, a = 1 mod pe
            long long r = 0, m = 1;
            for (int i = 0; i < k; i++) {
                long long ri = (mask >> i) & 1;
                long long mi = primes_powers[i];
                auto [nr, nm] = crt(r, m, ri, mi);
                r = nr;
                m = nm;
            }
            if (r > 0 && r < n) {
                best = max(best, r);
            }
        }

        total += best;
    }

    printf("%lld\n", total);
    return 0;
}
