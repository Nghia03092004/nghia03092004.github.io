#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 50000000;

    // Sieve of Eratosthenes up to sqrt(LIMIT) for squares
    int sieve_limit = 7072;
    vector<bool> is_prime(sieve_limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= sieve_limit; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= sieve_limit; j += i)
                is_prime[j] = false;
        }
    }

    vector<int> primes;
    for (int i = 2; i <= sieve_limit; i++)
        if (is_prime[i]) primes.push_back(i);

    vector<bool> seen(LIMIT, false);
    int count = 0;

    for (int r : primes) {
        long long r4 = (long long)r * r * r * r;
        if (r4 >= LIMIT) break;
        for (int q : primes) {
            long long q3 = (long long)q * q * q;
            if (r4 + q3 >= LIMIT) break;
            for (int p : primes) {
                long long p2 = (long long)p * p;
                long long sum = r4 + q3 + p2;
                if (sum >= LIMIT) break;
                if (!seen[sum]) {
                    seen[sum] = true;
                    count++;
                }
            }
        }
    }

    cout << count << endl;
    return 0;
}
