#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int N = 10000000;

    // Compute Euler's totient function using sieve
    vector<int> phi(N + 1);
    iota(phi.begin(), phi.end(), 0);

    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    // DP: f[n] = number of totient stairstep sequences ending at n
    vector<long long> f(N + 1, 1); // base case: single-element sequence
    f[0] = 0;

    long long total = 0;

    // For each n, propagate to multiples
    for (int n = 1; n <= N; n++) {
        total += f[n];
        for (long long m = 2LL * n; m <= N; m += n) {
            if (phi[m] > phi[n]) {
                f[m] += f[n];
            }
        }
    }

    cout << total << endl;
    // Expected: 7124517762917989

    return 0;
}
