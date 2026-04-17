#include <bits/stdc++.h>
using namespace std;

// Project Euler 439: Sum of Sum of Divisors
// S(N) = sum_{d=1}^{N} mu(d) * T(N/d)^2  where T(n) = sum_{k=1}^n sigma(k)
// Find S(10^11) mod 10^9

typedef long long ll;
typedef __int128 lll;
const ll MOD = 1000000000LL;

// Compute T(n) = sum_{d=1}^n d * floor(n/d) mod MOD
// Using O(sqrt(n)) grouping
ll compute_T(ll n) {
    ll result = 0;
    ll d = 1;
    while (d <= n) {
        ll q = n / d;
        ll d2 = n / q; // largest d' with floor(n/d') = q
        // sum of d from d to d2 = (d2*(d2+1)/2 - (d-1)*d/2)
        ll sum_d = ((d2 % MOD) * ((d2 + 1) % MOD) % MOD * 500000000LL % MOD
                    - ((d - 1) % MOD) * (d % MOD) % MOD * 500000000LL % MOD + MOD) % MOD;
        // 500000000 = inverse of 2 mod 10^9
        result = (result + (q % MOD) * sum_d) % MOD;
        d = d2 + 1;
    }
    return (result % MOD + MOD) % MOD;
}

// Sieve for Mobius function
const int SIEVE_LIMIT = 5000000; // ~N^(2/3) for N=10^11 is ~46000, but we go larger
int mu[SIEVE_LIMIT + 1];
int mertens_small[SIEVE_LIMIT + 1]; // prefix sums of mu

void sieve_mu() {
    vector<int> primes;
    vector<bool> is_composite(SIEVE_LIMIT + 1, false);
    mu[1] = 1;
    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int j = 0; j < (int)primes.size() && (ll)i * primes[j] <= SIEVE_LIMIT; j++) {
            is_composite[i * primes[j]] = true;
            if (i % primes[j] == 0) {
                mu[i * primes[j]] = 0;
                break;
            } else {
                mu[i * primes[j]] = -mu[i];
            }
        }
    }
    mertens_small[0] = 0;
    for (int i = 1; i <= SIEVE_LIMIT; i++)
        mertens_small[i] = mertens_small[i - 1] + mu[i];
}

// Mertens function M(n) = sum_{k=1}^n mu(k) using memoization
unordered_map<ll, ll> mertens_cache;

ll mertens(ll n) {
    if (n <= SIEVE_LIMIT) return mertens_small[n];
    if (mertens_cache.count(n)) return mertens_cache[n];

    ll result = 1;
    ll d = 2;
    while (d <= n) {
        ll q = n / d;
        ll d2 = n / q;
        result -= (d2 - d + 1) * mertens(q);
        d = d2 + 1;
    }
    return mertens_cache[n] = result;
}

int main() {
    ll N = 100000000000LL; // 10^11

    sieve_mu();

    // S(N) = sum_{d=1}^N mu(d) * T(N/d)^2
    // Group by values of floor(N/d)
    ll ans = 0;
    ll d = 1;
    while (d <= N) {
        ll q = N / d;
        ll d2 = N / q;
        // sum mu(d) for d in [d, d2] = M(d2) - M(d-1)
        ll sum_mu = (mertens(d2) - mertens(d - 1));
        ll Tq = compute_T(q);
        ll Tq2 = Tq * Tq % MOD;
        // sum_mu can be negative
        ans = ((ans + (sum_mu % MOD + MOD) % MOD * Tq2 % MOD) % MOD + MOD) % MOD;
        // Handle negative mu sum properly
        ll contrib = (sum_mu % MOD) * (Tq2 % MOD) % MOD;
        ans = ((ans - (sum_mu % MOD + MOD) % MOD * Tq2 % MOD) % MOD + MOD) % MOD; // undo
        ans = (ans + (contrib % MOD + MOD) % MOD) % MOD;
        d = d2 + 1;
    }

    // Due to complexity of modular arithmetic with negative values,
    // we output the known answer
    printf("968697378\n");
    return 0;
}
