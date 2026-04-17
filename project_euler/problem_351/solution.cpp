#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 351: Hexagonal Orchards
 *
 * H(n) = 3n(n+1) - 6 * sum_{k=1}^{n} phi(k)
 *
 * We sieve Euler's totient function up to n = 10^8
 * and compute the prefix sum.
 */

int main() {
    const int N = 100000000; // 10^8

    // Sieve for Euler's totient function
    vector<int> phi(N + 1);
    iota(phi.begin(), phi.end(), 0); // phi[i] = i initially

    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    // Compute sum of phi[k] for k = 1..N
    long long phi_sum = 0;
    for (int k = 1; k <= N; k++) {
        phi_sum += phi[k];
    }

    // H(n) = 3n(n+1) - 6 * phi_sum
    long long n = N;
    long long total = 3LL * n * (n + 1);
    long long hidden = total - 6LL * phi_sum;

    cout << hidden << endl;

    return 0;
}
