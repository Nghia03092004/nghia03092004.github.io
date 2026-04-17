#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 842: Irregular Clocking
 *
 * LCG: x_{n+1} = (a*x_n + c) mod m
 * Implements Floyd and Brent cycle detection,
 * plus matrix exponentiation for x_n at large indices.
 */

typedef long long ll;
typedef vector<vector<ll>> Mat;

Mat mat_mul(const Mat& A, const Mat& B, ll mod) {
    int n = A.size();
    Mat C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) if (A[i][k])
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
    return C;
}

Mat mat_pow(Mat M, ll p, ll mod) {
    int n = M.size();
    Mat result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = mat_mul(result, M, mod);
        M = mat_mul(M, M, mod);
        p >>= 1;
    }
    return result;
}

ll lcg_at(ll a, ll c, ll m, ll x0, ll n) {
    Mat M = {{a, c}, {0, 1}};
    Mat Mn = mat_pow(M, n, m);
    return (Mn[0][0] * x0 + Mn[0][1]) % m;
}

pair<ll, ll> brent_cycle(ll a, ll c, ll m, ll x0) {
    ll power = 1, lam = 1;
    ll tortoise = x0, hare = (a * x0 + c) % m;
    while (tortoise != hare) {
        if (power == lam) { tortoise = hare; power *= 2; lam = 0; }
        hare = (a * hare + c) % m;
        lam++;
    }
    tortoise = hare = x0;
    for (ll i = 0; i < lam; i++) hare = (a * hare + c) % m;
    ll tau = 0;
    while (tortoise != hare) {
        tortoise = (a * tortoise + c) % m;
        hare = (a * hare + c) % m;
        tau++;
    }
    return {tau, lam};
}

int main() {
    // Verify: a=2, c=0, m=7, x0=1 => period=3
    auto [t1, r1] = brent_cycle(2, 0, 7, 1);
    assert(r1 == 3);

    // Verify matrix exponentiation
    assert(lcg_at(2, 0, 7, 1, 0) == 1);
    assert(lcg_at(2, 0, 7, 1, 1) == 2);
    assert(lcg_at(2, 0, 7, 1, 3) == 1);

    // Full-period test: a=5,c=3,m=16
    auto [t2, r2] = brent_cycle(5, 3, 16, 0);
    assert(r2 == 16);

    ll ans = lcg_at(1103515245LL, 12345, 1LL << 31, 0, 1000000000000LL);
    cout << ans << endl;
    return 0;
}
