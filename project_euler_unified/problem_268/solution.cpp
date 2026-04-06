#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N = 10000000000000000LL; // 10^16
    // but the problem says "below 10^16", so we count numbers in [1, 10^16 - 1]
    // floor((N-1)/product) for divisibility count... actually floor((10^16-1)/p) = floor(10^16/p) - (p divides 10^16 ? 1 : 0)
    // Simpler: count of multiples of p in [1, N-1] = floor((N-1)/p)
    // But for large N, floor((10^16 - 1)/p) = floor(10^16/p) - 1 if p | 10^16, else floor(10^16/p)
    // Actually it's simpler: numbers below N = 10^16 means [1, 10^16 - 1]
    // count of multiples of d in [1, M] = floor(M/d)
    // So we use M = 10^16 - 1

    long long M = N - 1;

    vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
                          53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int np = primes.size(); // 25

    // Inclusion-exclusion: count of numbers with at least 4 distinct prime factors from the list
    // = sum_{k=4}^{25} (-1)^{k-4} * C(k-1, 3) * S_k
    // where S_k = sum over k-subsets of floor(M / product)

    // We'll enumerate subsets via recursion with pruning
    // For each subset of size k with product p, contribute (-1)^{k-4} * C(k-1,3) * floor(M/p)

    long long answer = 0;

    // Recursive enumeration
    // We process primes in order, building subsets
    function<void(int, int, long long)> solve = [&](int idx, int cnt, long long prod) {
        if (cnt >= 4) {
            // Contribute to answer
            long long term = M / prod;
            int sign = ((cnt - 4) % 2 == 0) ? 1 : -1;
            // C(cnt-1, 3)
            long long binom = 1;
            for (int i = 0; i < 3; i++) {
                binom *= (cnt - 1 - i);
                binom /= (i + 1);
            }
            answer += sign * binom * term;
        }

        for (int i = idx; i < np; i++) {
            // Check if product would overflow or exceed M
            if (prod > M / primes[i]) break; // primes are sorted, so all further products are larger
            solve(i + 1, cnt + 1, prod * primes[i]);
        }
    };

    solve(0, 0, 1);

    cout << answer << endl;
    return 0;
}
