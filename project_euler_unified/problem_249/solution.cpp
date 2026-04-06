#include <bits/stdc++.h>
using namespace std;

int main() {
    const unsigned long long MOD = 10000000000000000ULL; // 10^16

    // Generate all primes below 5000
    const int LIMIT = 5000;
    vector<bool> sieve(LIMIT, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (sieve[i])
            for (int j = i * i; j < LIMIT; j += i)
                sieve[j] = false;

    vector<int> primes;
    for (int i = 2; i < LIMIT; i++)
        if (sieve[i]) primes.push_back(i);

    long long S = 0;
    for (int p : primes) S += p;

    // Sieve up to S for final primality check
    vector<bool> is_prime(S + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i * i <= S; i++)
        if (is_prime[i])
            for (long long j = i * i; j <= S; j += i)
                is_prime[j] = false;

    // DP: 0/1 knapsack
    vector<unsigned long long> dp(S + 1, 0);
    dp[0] = 1;

    for (int p : primes) {
        for (long long s = S; s >= p; s--) {
            dp[s] += dp[s - p];
            if (dp[s] >= MOD) dp[s] -= MOD;
        }
    }

    unsigned long long answer = 0;
    for (long long s = 2; s <= S; s++) {
        if (is_prime[s]) {
            answer += dp[s];
            if (answer >= MOD) answer -= MOD;
        }
    }

    cout << answer << endl;
    return 0;
}
