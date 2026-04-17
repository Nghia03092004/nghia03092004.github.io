#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Totient sieve: compute phi(n) for all n in [0, N]
vector<int> totient_sieve(int N) {
    vector<int> phi(N + 1);
    iota(phi.begin(), phi.end(), 0);  // phi[n] = n
    for (int p = 2; p <= N; p++) {
        if (phi[p] == p) {  // p is prime
            for (int m = p; m <= N; m += p) {
                phi[m] = phi[m] / p * (p - 1);
            }
        }
    }
    return phi;
}

// Sum of phi(n^k) = sum of n^(k-1) * phi(n) for n = 1..N
ll sum_totient_powers(int N, int k) {
    auto phi = totient_sieve(N);
    ll total = 0;
    for (int n = 1; n <= N; n++) {
        ll nk = 1;
        for (int j = 0; j < k - 1; j++) nk *= n;  // n^(k-1)
        total += nk * phi[n];
    }
    return total;
}

// Sublinear sum of phi(n) for n = 1..N
// O(N^{2/3}) using the hyperbola method
ll sum_totient_sublinear(ll N) {
    if (N <= 1) return N;

    int cbrt_N = (int)cbrt((double)N) + 1;
    int small = min((ll)cbrt_N * cbrt_N, N);
    // If N is too large for sieve, limit small
    if (small > 5000000) small = 5000000;
    small = min(small, (int)N);

    auto phi = totient_sieve(small);
    vector<ll> prefix(small + 1, 0);
    for (int i = 1; i <= small; i++) {
        prefix[i] = prefix[i-1] + phi[i];
    }

    unordered_map<ll, ll> memo;

    function<ll(ll)> Phi = [&](ll n) -> ll {
        if (n <= small) return prefix[n];
        auto it = memo.find(n);
        if (it != memo.end()) return it->second;

        ll result = n * (n + 1) / 2;
        ll d = 2;
        while (d <= n) {
            ll q = n / d;
            ll d_next = n / q + 1;
            result -= (d_next - d) * Phi(q);
            d = d_next;
        }

        memo[n] = result;
        return result;
    };

    return Phi(N);
}

int main() {
    // Compute sum of phi(n) for n = 1..N
    int N = 10000;
    auto phi = totient_sieve(N);

    // Display first 20
    cout << "First 20 totient values:" << endl;
    for (int n = 1; n <= 20; n++) {
        cout << "  phi(" << n << ") = " << phi[n] << endl;
    }

    // Sum phi(n)
    ll sum1 = 0;
    for (int n = 1; n <= N; n++) sum1 += phi[n];
    cout << "\nSum phi(n) for n=1.." << N << ": " << sum1 << endl;

    // Sum phi(n^2) = sum n*phi(n)
    ll sum2 = sum_totient_powers(N, 2);
    cout << "Sum phi(n^2) for n=1.." << N << ": " << sum2 << endl;

    // Sublinear
    ll N_large = 1000000;
    ll sub = sum_totient_sublinear(N_large);
    cout << "\nSum phi(n) for n=1.." << N_large << ": " << sub << endl;

    return 0;
}
