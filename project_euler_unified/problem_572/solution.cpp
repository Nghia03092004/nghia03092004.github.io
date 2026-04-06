#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<vector<ll>> Matrix;

// Gaussian binomial coefficient [n choose k]_q
ll gaussian_binomial(int n, int k, ll q) {
    if (k < 0 || k > n) return 0;
    if (k == 0) return 1;
    ll num = 1, den = 1;
    for (int i = 0; i < k; i++) {
        ll qni = 1, qi1 = 1;
        for (int j = 0; j < n - i; j++) qni *= q;
        for (int j = 0; j < i + 1; j++) qi1 *= q;
        num *= (qni - 1);
        den *= (qi1 - 1);
    }
    return num / den;
}

ll power(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

// Count idempotent 3x3 matrices over F_p
ll count_idempotent_prime(ll p, int sz = 3) {
    ll total = 0;
    for (int r = 0; r <= sz; r++) {
        ll gb = gaussian_binomial(sz, r, p);
        total += gb * power(p, r * (sz - r));
    }
    return total;
}

// Count idempotent 3x3 matrices over Z/p^a Z
ll count_idempotent_pp(ll p, int a, int sz = 3) {
    ll total = 0;
    for (int r = 0; r <= sz; r++) {
        ll gb = gaussian_binomial(sz, r, p);
        ll base_count = gb * power(p, r * (sz - r));
        ll lift = power(p, (a - 1) * 2 * r * (sz - r));
        total += base_count * lift;
    }
    return total;
}

map<ll, int> factorize(ll n) {
    map<ll, int> f;
    for (ll d = 2; d * d <= n; d++) {
        while (n % d == 0) { f[d]++; n /= d; }
    }
    if (n > 1) f[n]++;
    return f;
}

int main() {
    ll total = 0;
    for (int n = 2; n <= 200; n++) {
        auto facs = factorize(n);
        ll In = 1;
        for (auto &[p, a] : facs) {
            In *= count_idempotent_pp(p, a);
        }
        total += In;
    }
    cout << total << endl;
    return 0;
}
