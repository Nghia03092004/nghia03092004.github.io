#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 370: Geometric Triangles
 *
 * Count triangles with integer sides a <= b <= c, b^2 = ac (geometric
 * progression), and a + b + c <= L.
 *
 * Parametrization: a = k*m^2, b = k*m*n, c = k*n^2 with gcd(m,n)=1, m<=n.
 * Triangle inequality: n/m < phi = (1+sqrt(5))/2.
 * Perimeter: k*(m^2+m*n+n^2) <= L.
 *
 * Sum over coprime (m,n) with m <= n < phi*m of floor(L/(m^2+m*n+n^2)).
 *
 * Answer: 41791929448408
 */

typedef long long ll;
typedef __int128 lll;

const double PHI = (1.0 + sqrt(5.0)) / 2.0;

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll L = 2500000000000LL; // 2.5 * 10^12

    ll total = 0;

    // Iterate over m from 1 upward
    // For each m, n ranges from m to floor(phi*m - epsilon)
    // We need m^2 + m*n + n^2 <= L (otherwise floor(L/q) = 0)
    // The minimum q is at n=m: 3m^2, so m <= sqrt(L/3)

    ll max_m = (ll)(sqrt((double)L / 3.0)) + 1;

    for (ll m = 1; m <= max_m; m++) {
        ll n_max = (ll)(PHI * m - 1e-9);
        // Also need m^2 + m*n + n^2 <= L for any contribution
        // Solve n^2 + m*n + m^2 - L <= 0
        // n <= (-m + sqrt(m^2 - 4*(m^2 - L))) / 2
        //    = (-m + sqrt(4L - 3m^2)) / 2
        double disc = 4.0 * L - 3.0 * (double)m * m;
        if (disc < 0) break;
        ll n_max2 = (ll)((-m + sqrt(disc)) / 2.0);
        if (n_max > n_max2) n_max = n_max2;
        if (n_max < m) continue;

        for (ll n = m; n <= n_max; n++) {
            if (gcd(m, n) != 1) continue;

            // Check triangle inequality: m^2 + m*n > n^2
            if (m * m + m * n <= n * n) continue;

            ll q = m * m + m * n + n * n;
            total += L / q;
        }
    }

    cout << total << endl;

    return 0;
}
