#include <bits/stdc++.h>
using namespace std;

bool is_permutation(int a, int b) {
    int digits_a[10] = {}, digits_b[10] = {};
    while (a > 0) { digits_a[a % 10]++; a /= 10; }
    while (b > 0) { digits_b[b % 10]++; b /= 10; }
    for (int i = 0; i < 10; i++)
        if (digits_a[i] != digits_b[i]) return false;
    return true;
}

int main() {
    const int LIMIT = 10000000;

    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i < LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    vector<int> primes;
    for (int i = 2; i < LIMIT; i++) {
        if (is_prime[i]) primes.push_back(i);
    }

    double best_ratio = 1e18;
    int best_n = 0;

    // Search semiprimes n = p * q with p <= q, pq < LIMIT
    for (int i = 0; i < (int)primes.size(); i++) {
        int p = primes[i];
        if ((long long)p * p >= LIMIT) break;
        for (int j = i; j < (int)primes.size(); j++) {
            int q = primes[j];
            long long n = (long long)p * q;
            if (n >= LIMIT) break;

            long long phi = (long long)(p - 1) * (q - 1);
            double ratio = (double)n / phi;

            if (ratio < best_ratio && is_permutation((int)n, (int)phi)) {
                best_ratio = ratio;
                best_n = (int)n;
            }
        }
    }

    cout << best_n << endl;

    return 0;
}
