#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

// Sum of multiples of d in open interval (lo, hi), i.e., lo < n < hi and d | n
// Returns sum as __int128 to avoid overflow
lll sum_multiples(ll d, ll lo, ll hi) {
    // First multiple of d greater than lo
    ll first_k = lo / d + 1;
    // Last multiple of d less than hi
    ll last_k = (hi - 1) / d;
    if (first_k > last_k) return 0;
    // Sum = d * (first_k + first_k+1 + ... + last_k) = d * (last_k - first_k + 1) * (first_k + last_k) / 2
    lll cnt = last_k - first_k + 1;
    lll s = cnt * (first_k + last_k) / 2;
    return (lll)d * s;
}

int main() {
    const ll LIMIT = 999966663333LL;
    const int SIEVE_SIZE = 1100000; // primes up to ~1.1M covers sqrt(LIMIT) ~ 999983

    // Sieve
    vector<bool> is_prime(SIEVE_SIZE + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (ll)i * i <= SIEVE_SIZE; i++)
        if (is_prime[i])
            for (int j = i * i; j <= SIEVE_SIZE; j += i)
                is_prime[j] = false;

    vector<ll> primes;
    for (int i = 2; i <= SIEVE_SIZE; i++)
        if (is_prime[i])
            primes.push_back(i);

    lll total = 0;

    for (int i = 0; i + 1 < (int)primes.size(); i++) {
        ll p = primes[i];
        ll q = primes[i + 1];
        ll lo = p * p;    // exclusive lower bound
        ll hi = q * q;    // exclusive upper bound (or LIMIT+1)

        if (lo >= LIMIT) break;
        if (hi > LIMIT + 1) hi = LIMIT + 1;

        // Sum of semidivisible in (lo, hi):
        // divisible by exactly one of p, q
        // = S_p + S_q - 2*S_{pq}
        lll sp = sum_multiples(p, lo, hi);
        lll sq = sum_multiples(q, lo, hi);
        lll spq = sum_multiples(p * q, lo, hi);
        total += sp + sq - 2 * spq;
    }

    // Print __int128
    // Convert to string
    ll hi_part = (ll)(total / 1000000000000000000LL);
    ll lo_part = (ll)(total % 1000000000000000000LL);
    if (hi_part > 0)
        printf("%lld%018lld\n", hi_part, lo_part);
    else
        printf("%lld\n", lo_part);

    return 0;
}
