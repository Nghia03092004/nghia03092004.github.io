/*
 * Problem 894: Summation of Summations
 * m-fold iterated partial sum: S^(m)_n = sum_k C(n-k+m-1, m-1) * a_k.
 * For a_k = k: S^(m)_n = C(n+m, m+1).
 * Three approaches: nested loops O(n^m), prefix sums O(mn), closed form O(n).
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll comb(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    if (r > n - r) r = n - r;
    ll result = 1;
    for (ll i = 0; i < r; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

// Method 1: Direct nested loops for m=3
ll triple_sum_brute(const vector<ll>& a) {
    int n = a.size();
    ll total = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k <= j; k++)
                total += a[k];
    return total;
}

// Method 2: Prefix sum applied m times
ll iterated_prefix(vector<ll> a, int m) {
    int n = a.size();
    for (int step = 0; step < m; step++)
        for (int i = 1; i < n; i++)
            a[i] += a[i - 1];
    return a[n - 1];
}

// Method 3: Closed-form weighted sum
ll closed_form(const vector<ll>& a, int m) {
    int n = a.size();
    ll total = 0;
    for (int k = 0; k < n; k++) {
        ll weight = comb(n - 1 - k + m - 1, m - 1);
        total += weight * a[k];
    }
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verification: a_k = k, m = 3
    cout << "=== Triple Summation (m=3, a_k=k) ===" << endl;
    for (int n = 1; n <= 10; n++) {
        vector<ll> a(n);
        for (int i = 0; i < n; i++) a[i] = i + 1;
        ll b = triple_sum_brute(a);
        ll p = iterated_prefix(a, 3);
        ll c = closed_form(a, 3);
        ll identity = comb(n + 3, 4);
        cout << "n=" << setw(2) << n
             << ": brute=" << setw(6) << b
             << " prefix=" << setw(6) << p
             << " formula=" << setw(6) << c
             << " C(n+3,4)=" << setw(6) << identity
             << (b == p && p == c && c == identity ? " OK" : " FAIL") << endl;
        assert(b == p && p == c && c == identity);
    }

    // General m verification
    cout << "\n=== Various m (a_k=k, n=10) ===" << endl;
    int n = 10;
    vector<ll> a(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;
    for (int m = 1; m <= 6; m++) {
        ll prefix = iterated_prefix(a, m);
        ll formula = closed_form(a, m);
        ll identity = comb(n + m, m + 1);
        cout << "m=" << m << ": prefix=" << prefix
             << " formula=" << formula
             << " C(n+m,m+1)=" << identity
             << (prefix == formula && formula == identity ? " OK" : " FAIL") << endl;
    }

    // Large computation
    ll ans = comb(10003, 4);
    cout << "\nS^(3)_{10000} for a_k=k = C(10003,4) = " << ans << endl;
    cout << "\nAnswer: " << ans << endl;

    return 0;
}
