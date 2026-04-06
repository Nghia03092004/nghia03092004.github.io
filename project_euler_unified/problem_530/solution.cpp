#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 530: GCD of Divisors
 *
 * f(n) = sum_{d|n} gcd(d, n/d)
 * F(k) = sum_{n=1}^{k} f(n)
 * Find F(10^15).
 *
 * F(k) = sum_{g=1}^{sqrt(k)} g * sum_{e: mu(e)!=0, g^2*e^2<=k} mu(e) * D(k/(g^2*e^2))
 * where D(x) = sum_{j=1}^{x} floor(x/j) (Dirichlet divisor sum)
 */

typedef long long ll;
typedef unsigned long long ull;

// Dirichlet divisor sum: D(x) = sum_{j=1}^{x} floor(x/j)
// Using hyperbola method: D(x) = 2*sum_{j=1}^{sqrt(x)} floor(x/j) - floor(sqrt(x))^2
ll dirichlet_divisor_sum(ll x) {
    if (x <= 0) return 0;
    ll sq = (ll)sqrt((double)x);
    while (sq * sq > x) sq--;
    while ((sq + 1) * (sq + 1) <= x) sq++;

    ll result = 0;
    for (ll j = 1; j <= sq; j++) {
        result += x / j;
    }
    result = 2 * result - sq * sq;
    return result;
}

int main() {
    // Verify small cases
    // F(10) = 32
    // Brute force for small values
    auto f_brute = [](int n) -> int {
        int sum = 0;
        for (int d = 1; d <= n; d++) {
            if (n % d == 0) {
                sum += __gcd(d, n / d);
            }
        }
        return sum;
    };

    ll F10 = 0;
    for (int n = 1; n <= 10; n++) F10 += f_brute(n);
    cout << "F(10) = " << F10 << endl;  // 32

    ll F1000 = 0;
    for (int n = 1; n <= 1000; n++) F1000 += f_brute(n);
    cout << "F(1000) = " << F1000 << endl;  // 12776

    // For F(10^15), use the formula with Mobius function
    ll N = 1000000000000000LL;  // 10^15
    ll sqN = (ll)sqrt((double)N);

    // Sieve Mobius function up to sqN
    int SIEVE_LIMIT = sqN + 10;
    vector<int> mu(SIEVE_LIMIT + 1, 1);
    vector<bool> is_prime(SIEVE_LIMIT + 1, true);
    vector<int> primes;

    // Compute Mobius function
    vector<int> min_factor(SIEVE_LIMIT + 1, 0);
    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            min_factor[i] = i;
            for (ll j = (ll)i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = false;
                if (min_factor[j] == 0) min_factor[j] = i;
            }
        }
    }

    // Compute mu properly
    mu[1] = 1;
    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        if (is_prime[i]) {
            mu[i] = -1;
        } else {
            int p = min_factor[i];
            if ((i / p) % p == 0) {
                mu[i] = 0;
            } else {
                mu[i] = -mu[i / p];
            }
        }
    }

    // F(N) = sum_{g=1}^{sqrt(N)} g * sum_{e: mu(e)!=0, g^2*e^2<=N} mu(e) * D(N/(g^2*e^2))
    ll answer = 0;
    for (ll g = 1; g * g <= N; g++) {
        ll limit_e2 = N / (g * g);
        ll limit_e = (ll)sqrt((double)limit_e2);
        while (limit_e * limit_e > limit_e2) limit_e--;
        while ((limit_e + 1) * (limit_e + 1) <= limit_e2) limit_e++;

        for (ll e = 1; e <= limit_e; e++) {
            if (mu[e] == 0) continue;
            ll M = N / (g * g * e * e);
            ll D = dirichlet_divisor_sum(M);
            answer += g * mu[e] * D;
        }
    }

    cout << "F(10^15) = " << answer << endl;  // 207366437157977206

    return 0;
}
