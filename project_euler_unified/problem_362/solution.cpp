#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 362: Squarefree Factors
 *
 * f(n) = largest squarefree divisor of n.
 * Compute sum of f(n) for n = 1 to 10^14.
 *
 * Approach: Write n = a^2 * b (b squarefree), then f(n) = b.
 * Use Mobius function for squarefree sum computation.
 *
 * Answer: 457895958010
 */

typedef long long ll;
typedef __int128 lll;

const ll N = 100000000000000LL; // 10^14

// Sieve smallest prime factor up to limit
vector<int> mu_sieve;
vector<int> mobius;

void compute_mobius(int limit) {
    mobius.assign(limit + 1, 0);
    mobius[1] = 1;
    vector<int> smallest_prime(limit + 1, 0);
    vector<int> primes;

    for (int i = 2; i <= limit; i++) {
        if (smallest_prime[i] == 0) {
            smallest_prime[i] = i;
            primes.push_back(i);
            mobius[i] = -1;
        }
        for (int j = 0; j < (int)primes.size() && primes[j] <= smallest_prime[i] && (ll)i * primes[j] <= limit; j++) {
            smallest_prime[i * primes[j]] = primes[j];
            if (i % primes[j] == 0) {
                mobius[i * primes[j]] = 0;
            } else {
                mobius[i * primes[j]] = -mobius[i];
            }
        }
    }
}

// T(m) = m*(m+1)/2, using 128-bit to avoid overflow
lll T(ll m) {
    return (lll)m * (m + 1) / 2;
}

// Sum of squarefree numbers up to x
lll squarefree_sum(ll x) {
    ll sq = (ll)sqrt((double)x);
    while ((sq + 1) * (sq + 1) <= x) sq++;
    while (sq * sq > x) sq--;

    lll result = 0;
    for (ll d = 1; d <= sq; d++) {
        if (mobius[d] == 0) continue;
        ll q = x / (d * d);
        result += (lll)mobius[d] * d * d * T(q);
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll sqrtN = (ll)sqrt((double)N);
    while ((sqrtN + 1) * (sqrtN + 1) <= N) sqrtN++;
    while (sqrtN * sqrtN > N) sqrtN--;

    // We need mobius values up to sqrt(N/1) = sqrt(N) ~ 10^7
    int mobius_limit = (int)sqrtN + 1;
    compute_mobius(mobius_limit);

    lll answer = 0;
    for (ll a = 1; a <= sqrtN; a++) {
        ll limit = N / (a * a);
        answer += squarefree_sum(limit);
    }

    cout << (ll)answer << endl;
    // Expected: 457895958010

    return 0;
}
