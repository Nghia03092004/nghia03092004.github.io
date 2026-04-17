#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 20000000;
    const int K = 15000000;
    const int M = N - K; // 5000000

    // Sieve of Eratosthenes
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }

    // For each prime, compute exponent in C(N, K) using Legendre's formula
    long long answer = 0;
    for (int p = 2; p <= N; p++) {
        if (!is_prime[p]) continue;
        long long exp_val = 0;
        long long pk = p;
        while (pk <= N) {
            exp_val += N / pk - K / pk - M / pk;
            if (pk > N / p) break; // prevent overflow
            pk *= p;
        }
        answer += exp_val * p;
    }

    cout << answer << endl;
    return 0;
}
