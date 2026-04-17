#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 263: An Engineers' Dream Come True
 *
 * Find first 4 "engineers' paradises" n such that:
 * - n-9, n-3, n+3, n+9 are all prime (sexy prime quadruplet)
 * - (n-9, n-3), (n-3, n+3), (n+3, n+9) are CONSECUTIVE prime pairs
 *   (no primes between them: n-7, n-5, n-1, n+1, n+5, n+7 all composite)
 * - n-8, n-4, n, n+4, n+8 are all practical numbers
 *
 * n ≡ 10 or 20 (mod 30).
 *
 * Answer: 2039506520
 */

const int LIMIT = 1200000010;
vector<bool> is_prime_sieve;

void build_sieve() {
    is_prime_sieve.assign(LIMIT + 1, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (ll i = 2; i * i <= LIMIT; i++) {
        if (is_prime_sieve[i]) {
            for (ll j = i * i; j <= LIMIT; j += i)
                is_prime_sieve[j] = false;
        }
    }
}

bool is_practical(ll n) {
    if (n <= 0) return false;
    if (n == 1) return true;
    if (n % 2 != 0) return false;
    vector<pair<ll,int>> factors;
    ll temp = n;
    for (ll d = 2; d * d <= temp; d++) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) { temp /= d; e++; }
            factors.push_back({d, e});
        }
    }
    if (temp > 1) factors.push_back({temp, 1});
    sort(factors.begin(), factors.end());
    if (factors[0].first != 2) return false;
    ll sigma = 1;
    for (int i = 0; i < (int)factors.size(); i++) {
        ll p = factors[i].first;
        int a = factors[i].second;
        if (i == 0) {
            ll pw = 1;
            for (int j = 0; j <= a; j++) pw *= p;
            sigma = (pw - 1) / (p - 1);
        } else {
            if (p > 1 + sigma) return false;
            ll pw = 1;
            for (int j = 0; j <= a; j++) pw *= p;
            sigma *= (pw - 1) / (p - 1);
        }
    }
    return true;
}

int main() {
    fprintf(stderr, "Building sieve up to %d...\n", LIMIT);
    build_sieve();
    fprintf(stderr, "Sieve done.\n");

    vector<ll> results;
    for (ll n = 10; n <= 1200000000LL && (int)results.size() < 4; n += 10) {
        int r = n % 30;
        if (r != 10 && r != 20) continue;

        // Check four primes
        if (!is_prime_sieve[n-9] || !is_prime_sieve[n+9] ||
            !is_prime_sieve[n-3] || !is_prime_sieve[n+3])
            continue;

        // Check consecutive: no primes between the pairs
        // n-7, n-5 must be composite (between n-9 and n-3)
        // n-1, n+1 must be composite (between n-3 and n+3)
        // n+5, n+7 must be composite (between n+3 and n+9)
        if (is_prime_sieve[n-7] || is_prime_sieve[n-5] ||
            is_prime_sieve[n-1] || is_prime_sieve[n+1] ||
            is_prime_sieve[n+5] || is_prime_sieve[n+7])
            continue;

        // Check five practical numbers
        if (is_practical(n-8) && is_practical(n-4) &&
            is_practical(n) && is_practical(n+4) &&
            is_practical(n+8)) {
            results.push_back(n);
            fprintf(stderr, "Found n = %lld (%lu/4)\n", n, results.size());
        }
    }

    ll ans = 0;
    for (ll x : results) ans += x;
    cout << ans << endl;
    return 0;
}
