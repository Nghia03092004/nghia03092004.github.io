#include <bits/stdc++.h>
using namespace std;

int main() {
    // We need to count pairs (p, q) with p < q both prime such that
    // p^q * q^p <= 800800^800800
    // Equivalently: q*ln(p) + p*ln(q) <= 800800*ln(800800)

    const int LIMIT = 800800;
    const double L = (double)LIMIT * log((double)LIMIT);

    // Sieve of Eratosthenes
    // Upper bound: q can be at most around LIMIT (when p=2, q*ln2 <= L => q ~ L/ln2 ~ 800800*ln(800800)/ln(2))
    // ln(800800) ~ 13.59, so q_max ~ 800800*13.59/0.693 ~ 15.7M
    // But we need a tighter bound. For p=2: q*ln(2) + 2*ln(q) <= L
    // q ~ L/ln(2) ~ 800800*13.59/0.693 ~ 15,700,000
    const int SIEVE_LIMIT = 16000000;

    vector<bool> is_prime(SIEVE_LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    vector<int> primes;
    primes.reserve(1100000);
    for (int i = 2; i <= SIEVE_LIMIT; i++)
        if (is_prime[i])
            primes.push_back(i);

    long long count = 0;
    int n = primes.size();

    // Two-pointer approach: for increasing p, q_max decreases
    int right = n - 1;

    for (int i = 0; i < n; i++) {
        long long p = primes[i];
        double lnp = log((double)p);

        // We need q > p, so j > i
        // q*ln(p) + p*ln(q) <= L
        // As p increases, the max valid q decreases
        if (right <= i) right = i + 1;

        // Shrink right pointer
        while (right >= 0 && right > i) {
            long long q = primes[right];
            double val = (double)q * lnp + (double)p * log((double)q);
            if (val <= L) break;
            right--;
        }

        if (right <= i) break; // no valid q for this p

        count += (long long)(right - i);
    }

    cout << count << endl;

    return 0;
}
