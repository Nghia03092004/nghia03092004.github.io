#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 100;

    // Sieve of Eratosthenes
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= N; i++)
        if (is_prime[i])
            for (int j = i * i; j <= N; j += i)
                is_prime[j] = false;

    vector<int> primes;
    for (int i = 2; i <= N; i++)
        if (is_prime[i])
            primes.push_back(i);

    // DP: unbounded knapsack with prime denominations
    vector<long long> dp(N + 1, 0);
    dp[0] = 1;
    for (int p : primes)
        for (int j = p; j <= N; j++)
            dp[j] += dp[j - p];

    // Find first n with q(n) > 5000
    for (int n = 2; n <= N; n++) {
        if (dp[n] > 5000) {
            cout << n << endl;
            return 0;
        }
    }

    return 0;
}
