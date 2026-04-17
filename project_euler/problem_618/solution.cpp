#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1000000007;

int main() {
    int N = 100;
    vector<int> primes;
    vector<bool> sieve(N + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i <= N; i++) {
        if (sieve[i]) {
            primes.push_back(i);
            for (int j = 2*i; j <= N; j += i) sieve[j] = false;
        }
    }
    
    vector<ll> dp(N + 1, 0);
    dp[0] = 1;
    for (int p : primes) {
        for (int n = p; n <= N; n++)
            dp[n] = (dp[n] + dp[n-p] * p) % MOD;
    }
    
    ll S = 0;
    for (int p : primes)
        S = (S + dp[p]) % MOD;
    cout << S << endl;
    return 0;
}
