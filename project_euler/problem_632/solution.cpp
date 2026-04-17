#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 632: Sum of Divisors of Sum of Divisors
 *
 * sigma(n) = sum of divisors.
 * Compute S(N) = sum_{n=1}^{N} sigma(sigma(n)).
 *
 * Method 1: Sieve for sigma, then lookup/compute
 * Method 2: Trial division (verification)
 */

const int MAXN = 100001;
ll sig[MAXN];

void sieve_sigma(int N) {
    memset(sig, 0, sizeof(sig));
    for (int d = 1; d <= N; d++)
        for (int m = d; m <= N; m += d)
            sig[m] += d;
}

ll sigma_trial(ll n) {
    ll total = 0;
    for (ll d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            total += d;
            if (d != n / d) total += n / d;
        }
    }
    return total;
}

int main() {
    int N = 100000;
    sieve_sigma(N);

    // Verify
    for (int n = 1; n <= 1000; n++)
        assert(sig[n] == sigma_trial(n));

    // Compute S(N)
    ll S = 0;
    for (int n = 1; n <= N; n++) {
        ll sn = sig[n];
        if (sn <= N) S += sig[sn];
        else S += sigma_trial(sn);
    }

    cout << "S(" << N << ") = " << S << endl;
    return 0;
}
