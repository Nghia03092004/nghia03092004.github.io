#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 100000000;
    const int SIEVE_SIZE = LIMIT / 2; // max q when p=2

    // Sieve of Eratosthenes
    vector<bool> is_composite(SIEVE_SIZE, false);
    vector<int> primes;

    for (int i = 2; i < SIEVE_SIZE; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            if ((long long)i * i < SIEVE_SIZE) {
                for (long long j = (long long)i * i; j < SIEVE_SIZE; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }

    // For each prime p, count primes q >= p with p*q < LIMIT
    long long count = 0;
    int n = primes.size();

    for (int i = 0; i < n; i++) {
        long long p = primes[i];
        if (p * p >= LIMIT) break;

        // Find largest index j such that primes[j] < LIMIT / p
        // and primes[j] >= p
        long long max_q = (LIMIT - 1) / p;

        // Binary search for max_q in primes
        int lo = i, hi = n - 1, best = i - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (primes[mid] <= max_q) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        if (best >= i) {
            count += (best - i + 1);
        }
    }

    cout << count << endl;
    return 0;
}
