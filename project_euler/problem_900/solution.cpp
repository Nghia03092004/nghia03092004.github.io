/*
 * Problem 900: Sum of Remainders
 *
 * Compute S(n) = sum_{k=1}^{n} (n mod k)
 *            = n^2 - sum_{k=1}^{n} k * floor(n/k)
 *
 * Uses the Dirichlet hyperbola method: floor(n/k) takes O(sqrt(n)) distinct
 * values, so we iterate over blocks of constant floor value.
 *
 * Complexity: O(sqrt(n)) time, O(1) space.
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

// Brute force O(n) -- for verification
ll S_brute(ll n) {
    ll s = 0;
    for (ll k = 1; k <= n; k++)
        s += n % k;
    return s;
}

// O(sqrt(n)) via floor-block decomposition
ll S_hyperbola(ll n) {
    ll T = 0;
    ll k = 1;
    while (k <= n) {
        ll v = n / k;
        ll kp = n / v;  // last index in this block
        // Sum of k + (k+1) + ... + kp = (kp - k + 1) * (k + kp) / 2
        ll block_len = kp - k + 1;
        ll block_sum = block_len * (k + kp) / 2;
        T += v * block_sum;
        k = kp + 1;
    }
    return n * n - T;
}

// O(sqrt(n)) with 128-bit arithmetic for very large n
lll S_hyperbola_128(lll n) {
    lll T = 0;
    lll k = 1;
    while (k <= n) {
        lll v = n / k;
        lll kp = n / v;
        lll block_len = kp - k + 1;
        lll block_sum = block_len * (k + kp) / 2;
        T += v * block_sum;
        k = kp + 1;
    }
    return n * n - T;
}

void print_128(lll x) {
    if (x == 0) { cout << "0"; return; }
    if (x < 0) { cout << "-"; x = -x; }
    string s;
    while (x > 0) { s += char('0' + (int)(x % 10)); x /= 10; }
    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verification against brute force
    cout << "=== Verification ===" << endl;
    cout << setw(8) << "n" << setw(12) << "Brute" << setw(12) << "Hyperbola"
         << setw(8) << "Match" << endl;
    for (ll n : {1LL, 2LL, 5LL, 10LL, 20LL, 50LL, 100LL, 500LL, 1000LL}) {
        ll sb = S_brute(n);
        ll sh = S_hyperbola(n);
        cout << setw(8) << n << setw(12) << sb << setw(12) << sh
             << setw(8) << (sb == sh ? "OK" : "FAIL") << endl;
        assert(sb == sh);
    }

    // Large values
    cout << "\n=== Large computations ===" << endl;
    for (ll n : {1000000LL, 10000000LL, 100000000LL, 1000000000LL}) {
        ll val = S_hyperbola(n);
        double ratio = (double)val / ((double)n * n);
        cout << "S(" << n << ") = " << val
             << ", S/n^2 = " << fixed << setprecision(6) << ratio << endl;
    }

    cout << "\nTheoretical limit: 1 - pi^2/12 = "
         << fixed << setprecision(6) << 1.0 - M_PI * M_PI / 12.0 << endl;

    // Final answer
    ll ans = S_hyperbola(100000000LL);
    cout << "\nAnswer: S(10^8) = " << ans << endl;

    return 0;
}
