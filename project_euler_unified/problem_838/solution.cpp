#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 838: Not Coprime
 *
 * Count pairs (a,b) with 1 <= a <= b <= N and gcd(a,b) > 1.
 *
 * Method: Mobius sieve + block summation.
 *   Phi(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2  (ordered coprime pairs)
 *   C(N) = N(N+1)/2 - (Phi(N)+1)/2
 *
 * Two methods implemented for cross-verification.
 */

const int MAXN = 10000001;

int mu[MAXN];
long long M[MAXN];  // Mertens function prefix sums
bool is_prime[MAXN];
vector<int> primes;

void sieve_mobius(int n) {
    fill(is_prime, is_prime + n + 1, true);
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if ((long long)i * p > n) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
    // Compute Mertens function
    M[0] = 0;
    for (int i = 1; i <= n; i++) {
        M[i] = M[i - 1] + mu[i];
    }
}

// Method 1: Block summation using Mertens prefix sums
long long solve_block(int N) {
    long long phi_N = 0;
    int d = 1;
    while (d <= N) {
        long long q = N / d;
        int d2 = N / q;
        phi_N += q * q * (M[d2] - M[d - 1]);
        d = d2 + 1;
    }
    long long total = (long long)N * (N + 1) / 2;
    long long coprime = (phi_N + 1) / 2;
    return total - coprime;
}

// Method 2: Direct summation (slower, for verification)
long long solve_direct(int N) {
    long long phi_N = 0;
    for (int d = 1; d <= N; d++) {
        long long q = N / d;
        phi_N += (long long)mu[d] * q * q;
    }
    long long total = (long long)N * (N + 1) / 2;
    long long coprime = (phi_N + 1) / 2;
    return total - coprime;
}

int main() {
    int N = 10000000;
    sieve_mobius(N);

    long long ans1 = solve_block(N);
    long long ans2 = solve_direct(N);

    assert(ans1 == ans2);
    cout << ans1 << endl;
    return 0;
}
