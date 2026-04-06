#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 408: Admissible Paths Through a Grid
 *
 * Inadmissible point (x,y): x, y, x+y all positive perfect squares.
 * This means x=a^2, y=b^2, a^2+b^2=c^2 (Pythagorean triple).
 *
 * P(n) = admissible paths from (0,0) to (n,n).
 * Use inclusion-exclusion DP over inadmissible points.
 */

const long long MOD = 1000000007;
const int MAXF = 20000002;

long long fac[MAXF], inv_fac[MAXF];

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

void precompute_factorials(int n) {
    fac[0] = 1;
    for (int i = 1; i <= n; i++)
        fac[i] = fac[i-1] * i % MOD;
    inv_fac[n] = power(fac[n], MOD - 2, MOD);
    for (int i = n - 1; i >= 0; i--)
        inv_fac[i] = inv_fac[i+1] * (i+1) % MOD;
}

long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fac[n] % MOD * inv_fac[k] % MOD * inv_fac[n-k] % MOD;
}

int main() {
    ios_base::sync_with_stdio(false);

    const int N = 10000000;
    precompute_factorials(2 * N);

    // Generate all Pythagorean triples (a,b,c) with a^2 <= N and b^2 <= N
    // Inadmissible points: (a^2, b^2) where a^2 + b^2 = c^2
    // Generate all primitive triples using Euclid's formula and scale
    int sq = (int)sqrt((double)N);

    set<pair<int,int>> pts_set;

    // Euclid's formula: a = m^2 - n^2, b = 2mn, c = m^2 + n^2
    // where m > n > 0, gcd(m,n) = 1, m-n odd
    for (int m = 1; (long long)m * m <= 2LL * N; m++) {
        for (int n = 1; n < m; n++) {
            if ((m - n) % 2 == 0) continue;
            if (__gcd(m, n) != 1) continue;

            long long a0 = (long long)m * m - (long long)n * n;
            long long b0 = 2LL * m * n;

            // Scale by k: triple is (k*a0, k*b0, k*c0)
            // Need (k*a0)^2 <= N and (k*b0)^2 <= N
            for (long long k = 1; ; k++) {
                long long a = k * a0, b = k * b0;
                if (a * a > N || b * b > N) {
                    if (a * a > N && b * b > N) break;
                    // Only one exceeds, but we also try swapped
                    if (b * b <= N && a * a <= N) {
                        // both ok
                    }
                    // Check both orderings
                    if (a * a <= N && b * b <= N)
                        pts_set.insert({(int)(a*a), (int)(b*b)});
                    if (b * b <= N && a * a <= N)
                        pts_set.insert({(int)(b*b), (int)(a*a)});
                    break;
                }
                pts_set.insert({(int)(a*a), (int)(b*b)});
                pts_set.insert({(int)(b*b), (int)(a*a)});
            }
        }
    }

    // Convert to sorted vector
    vector<pair<int,int>> pts(pts_set.begin(), pts_set.end());
    // Sort by x+y, then by x
    sort(pts.begin(), pts.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        int sa = a.first + a.second, sb = b.first + b.second;
        if (sa != sb) return sa < sb;
        return a.first < b.first;
    });

    int m = pts.size();

    // DP: f[i] = # paths from (0,0) to pts[i] avoiding all other inadmissible points
    vector<long long> f(m);
    for (int i = 0; i < m; i++) {
        int xi = pts[i].first, yi = pts[i].second;
        f[i] = C(xi + yi, xi);
        for (int j = 0; j < i; j++) {
            int xj = pts[j].first, yj = pts[j].second;
            if (xj <= xi && yj <= yi) {
                f[i] = (f[i] - f[j] % MOD * C(xi - xj + yi - yj, xi - xj) % MOD + MOD) % MOD;
            }
        }
    }

    // P(N) = C(2N, N) - sum f[i] * C(2N - xi - yi, N - xi)
    long long ans = C(2 * N, N);
    for (int i = 0; i < m; i++) {
        int xi = pts[i].first, yi = pts[i].second;
        ans = (ans - f[i] % MOD * C(2 * N - xi - yi, N - xi) % MOD + MOD) % MOD;
    }

    printf("%lld\n", ans);
    return 0;
}
