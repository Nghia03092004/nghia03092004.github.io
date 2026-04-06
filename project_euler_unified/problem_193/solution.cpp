#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count squarefree numbers below 2^50
    // Q(N) = sum_{k=1}^{sqrt(N)} mu(k) * floor(N/k^2)
    const long long N = (1LL << 50) - 1;
    const int SQ = 1 << 25; // sqrt(2^50) = 2^25

    // Sieve Mobius function
    vector<int> mu(SQ + 1, 1);
    vector<bool> is_prime(SQ + 1, true);
    vector<int> primes;

    // We'll use a sieve that tracks the smallest prime factor
    // and whether a number is divisible by p^2 for any prime p
    // Simple approach: for each prime p, multiply mu by -1 for multiples of p,
    // set mu=0 for multiples of p^2

    for (int i = 2; i <= SQ; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            // i is prime
            for (long long j = i; j <= SQ; j += i) {
                is_prime[j] = (j == i);
                mu[j] *= -1;
            }
            long long i2 = (long long)i * i;
            for (long long j = i2; j <= SQ; j += i2) {
                mu[j] = 0;
            }
        }
    }

    long long ans = 0;
    for (int k = 1; k <= SQ; k++) {
        if (mu[k] != 0) {
            ans += (long long)mu[k] * (N / ((long long)k * k));
        }
    }

    cout << ans << endl;
    return 0;
}
