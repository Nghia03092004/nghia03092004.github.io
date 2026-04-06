#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

ll phi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

double log_multinomial(int m, int t) {
    double result = 0;
    for (int i = 1; i <= m * t; i++) result += log((double)i);
    for (int i = 1; i <= t; i++) result -= m * log((double)i);
    return result;
}

// Compute (m*t)! / (t!)^m via iterated binomial coefficients
lll exact_multinomial(int m, int t) {
    lll result = 1;
    int total = m * t;
    for (int k = 0; k < m; k++) {
        int remaining = total - k * t;
        for (int i = 0; i < t; i++) {
            result *= (remaining - i);
            result /= (i + 1);
        }
    }
    return result;
}

ll f(int m, int n) {
    ll total_beads = (ll)m * n;
    lll result = 0;
    for (int t = 1; t <= n; t++) {
        if (n % t != 0) continue;
        double log_val = log_multinomial(m, t);
        if (log_val > 45) return -1;
        lll multi = exact_multinomial(m, t);
        result += (lll)phi(n / t) * multi;
    }
    result /= total_beads;
    if (result > (lll)1000000000000000LL) return -1;
    return (ll)result;
}

int main() {
    ll LIMIT = 1000000000000000LL;
    ll total = 0;
    for (int m = 2; m <= 18; m++) {
        for (int n = 1; n <= 29; n++) {
            ll val = f(m, n);
            if (val >= 0 && val <= LIMIT)
                total += val;
        }
    }
    cout << total << endl;
    return 0;
}
