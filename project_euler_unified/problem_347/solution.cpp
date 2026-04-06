#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long N = 10000000;

    // Sieve of Eratosthenes
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i * i <= N; i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }

    vector<long long> primes;
    for (long long i = 2; i <= N; i++) {
        if (is_prime[i]) primes.push_back(i);
    }

    long long total = 0;

    for (int i = 0; i < (int)primes.size(); i++) {
        long long p = primes[i];
        if (p * p > N) break; // p < q, so p*q > p*p

        for (int j = i + 1; j < (int)primes.size(); j++) {
            long long q = primes[j];
            if (p * q > N) break;

            long long best = 0;
            // Enumerate p^a * q^b with a>=1, b>=1
            long long pa = p;
            while (pa * q <= N) {
                long long val = pa * q;
                // Multiply by q as much as possible
                while (val * q <= N) val *= q;
                best = max(best, val);
                pa *= p;
            }

            total += best;
        }
    }

    cout << total << endl;
    return 0;
}
