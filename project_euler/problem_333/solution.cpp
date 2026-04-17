#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 333: Special Partitions
 *
 * Find the sum of all primes < 10^6 that can be written in exactly one way
 * as a sum of distinct terms of the form 2^a * 3^b.
 *
 * Approach:
 * 1. Generate all numbers 2^a * 3^b < 10^6.
 * 2. 0-1 knapsack DP to count representations (capped at 2).
 * 3. Sieve primes, sum those with exactly 1 representation.
 *
 * Answer: 3053105
 */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int LIMIT = 1000000;

    // Generate all 2^a * 3^b < LIMIT
    vector<int> terms;
    for (long long b3 = 1; b3 < LIMIT; b3 *= 3) {
        for (long long val = b3; val < LIMIT; val *= 2) {
            terms.push_back((int)val);
        }
    }
    sort(terms.begin(), terms.end());

    cout << "Number of terms: " << terms.size() << endl;

    // 0-1 knapsack: count representations, cap at 2
    // dp[s] = number of ways to represent s (capped at 2)
    vector<unsigned char> dp(LIMIT, 0);
    dp[0] = 1;

    for (int t : terms) {
        // Process in reverse for 0-1 knapsack
        for (int s = LIMIT - 1 - t; s >= 0; s--) {
            if (dp[s] > 0 && dp[s + t] < 2) {
                dp[s + t] = min(2, (int)dp[s + t] + (int)dp[s]);
            }
        }
    }

    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    // Sum primes with exactly 1 representation
    long long answer = 0;
    int count = 0;
    for (int p = 2; p < LIMIT; p++) {
        if (is_prime[p] && dp[p] == 1) {
            answer += p;
            count++;
        }
    }

    cout << "Number of special primes: " << count << endl;
    cout << "Answer: " << answer << endl;

    return 0;
}
