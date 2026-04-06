#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 278: Linear Combinations of Semiprimes
 *
 * For distinct primes p, q, r, f(p,q,r) = 2pqr - pq - qr - rp.
 * Sum f over all unordered triples {p,q,r} of distinct primes up to 5000.
 *
 * Optimization: for fixed p < q, sum over all primes r > q (up to 5000):
 *   sum_r f(p,q,r) = (2pq - p - q) * (sum of r) - pq * (count of r)
 *
 * Use prefix sums of primes for O(1) per (p,q) pair.
 * Uses __int128 to handle the large answer (~1.2 * 10^18).
 */

int main(){
    const int LIMIT = 5000;

    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; i++)
        if (is_prime[i])
            for (int j = i*i; j <= LIMIT; j += i)
                is_prime[j] = false;

    vector<int> primes;
    for (int i = 2; i <= LIMIT; i++)
        if (is_prime[i])
            primes.push_back(i);

    // Prefix sums and counts of primes
    vector<long long> ps(LIMIT + 1, 0);
    vector<int> pc(LIMIT + 1, 0);
    for (int i = 1; i <= LIMIT; i++) {
        ps[i] = ps[i-1] + (is_prime[i] ? i : 0);
        pc[i] = pc[i-1] + (is_prime[i] ? 1 : 0);
    }

    __int128 answer = 0;
    int n = primes.size();

    for (int i = 0; i < n; i++) {
        long long p = primes[i];
        for (int j = i + 1; j < n; j++) {
            long long q = primes[j];

            // r ranges over all primes > q and <= LIMIT
            long long sumR = ps[LIMIT] - ps[q];
            long long countR = pc[LIMIT] - pc[q];
            if (countR == 0) continue;

            __int128 coeff = (__int128)2 * p * q - p - q;
            __int128 contribution = coeff * sumR - (__int128)(p * q) * countR;
            answer += contribution;
        }
    }

    // Print __int128
    string s;
    __int128 x = answer;
    if (x == 0) { cout << 0 << endl; return 0; }
    while (x > 0) { s += '0' + (int)(x % 10); x /= 10; }
    reverse(s.begin(), s.end());
    cout << s << endl;

    return 0;
}
