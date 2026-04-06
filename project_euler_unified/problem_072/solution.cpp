#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count reduced proper fractions n/d with d <= 1,000,000.
    // Answer = sum of phi(d) for d = 2 to N (Theorem 3).
    // Euler totient sieve in O(N log log N).

    const int N = 1000000;
    vector<int> phi(N + 1);
    iota(phi.begin(), phi.end(), 0);  // phi[i] = i

    for (int p = 2; p <= N; p++) {
        if (phi[p] == p) {  // p is prime
            for (int m = p; m <= N; m += p) {
                phi[m] = phi[m] / p * (p - 1);
            }
        }
    }

    long long total = 0;
    for (int d = 2; d <= N; d++) {
        total += phi[d];
    }

    cout << total << endl;
    return 0;
}
