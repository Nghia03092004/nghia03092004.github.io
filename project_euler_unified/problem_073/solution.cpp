#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count reduced fractions n/d with 1/3 < n/d < 1/2, d <= 12000.
    // Mobius summation: C(N) = sum_k mu(k) * G(floor(N/k)).

    const int N = 12000;

    // Sieve the Mobius function
    vector<int> mu(N + 1, 1);
    vector<bool> is_prime(N + 1, true);

    for (int p = 2; p <= N; p++) {
        if (is_prime[p]) {
            for (int m = 2 * p; m <= N; m += p)
                is_prime[m] = false;
            for (long long m = (long long)p * p; m <= N; m += (long long)p * p)
                mu[m] = 0;
            for (int m = p; m <= N; m += p)
                mu[m] *= -1;
        }
    }

    long long count = 0;
    for (int k = 1; k <= N; k++) {
        if (mu[k] == 0) continue;
        int limit = N / k;
        long long c = 0;
        for (int d = 2; d <= limit; d++) {
            int lo = d / 3 + 1;
            int hi = (d - 1) / 2;
            if (hi >= lo) c += hi - lo + 1;
        }
        count += mu[k] * c;
    }

    cout << count << endl;
    return 0;
}
