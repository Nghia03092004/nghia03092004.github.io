#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 927: Totient Sum Optimization
 *
 * Find sum_{k=1}^{10^7} phi(k) mod 10^9+7.
 *
 * Euler sieve: phi(n) = n * prod_{p|n} (1 - 1/p).
 * Initialize phi[n] = n, then for each prime p:
 *   phi[m] -= phi[m]/p for all m divisible by p.
 *
 * Key identity: sum_{d|n} phi(d) = n.
 * Asymptotic: Phi(N) = 3N^2/pi^2 + O(N log N).
 *
 * Complexity: O(N log log N) sieve, O(N) summation.
 */

int main() {
    const int N = 10000000;
    const long long MOD = 1000000007;
    vector<int> phi(N + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= N; p++) {
        if (phi[p] == p) { // p is prime
            for (int m = p; m <= N; m += p) {
                phi[m] -= phi[m] / p;
            }
        }
    }
    long long ans = 0;
    for (int k = 1; k <= N; k++) {
        ans = (ans + phi[k]) % MOD;
    }
    cout << ans << endl;

    // Verify: phi(1)=1, phi(2)=1, phi(6)=2, phi(12)=4
    assert(phi[1] == 1);
    assert(phi[2] == 1);
    assert(phi[6] == 2);
    assert(phi[12] == 4);

    return 0;
}
