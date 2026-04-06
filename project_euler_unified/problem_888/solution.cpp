/*
 * Problem 888: Multidimensional Sieve
 * Mobius function sieve and coprime tuple counting.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<int> mobius_sieve(int N) {
    vector<int> mu(N + 1, 0), primes;
    vector<bool> is_prime(N + 1, true);
    mu[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (is_prime[i]) { primes.push_back(i); mu[i] = -1; }
        for (int p : primes) {
            if ((ll)i * p > N) break;
            is_prime[i * p] = false;
            if (i % p == 0) { mu[i * p] = 0; break; }
            else mu[i * p] = -mu[i];
        }
    }
    return mu;
}

ll coprime_pairs(int N, const vector<int>& mu) {
    ll total = 0;
    for (int d = 1; d <= N; d++)
        total += (ll)mu[d] * (N / d) * (N / d);
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N = 1000;
    auto mu = mobius_sieve(N);

    cout << "=== Mobius Function ===" << endl;
    for (int n = 1; n <= 30; n++)
        cout << "mu(" << n << ") = " << mu[n] << endl;

    cout << "\n=== Coprime Pairs ===" << endl;
    for (int n : {10, 50, 100, 500, 1000})
        cout << "C_2(" << n << ") = " << coprime_pairs(n, mu) << endl;

    cout << "\nAnswer: C_2(1000) = " << coprime_pairs(1000, mu) << endl;
    return 0;
}
