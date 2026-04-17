#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 403: Lattice Points Enclosed by Parabola and Line
 *
 * Area of D(a,b) = (a^2+4b)^{3/2}/6, rational iff a^2+4b = s^2 (perfect square).
 * L(a,b) depends only on s: L(s) = (s+1)(s^2-s+6)/6.
 * Reparametrize: a = s+2u, b = u(s+u). Sum over (u,s) pairs.
 * Use prefix sum P(K) = (K+1)(K+2)(K^2-K+12)/24 and floor(N/v) grouping
 * for O(sqrt(N)) complexity.
 *
 * Answer: S(10^12) mod 10^8 = 18224771
 */

typedef long long ll;
typedef __int128 lll;

const ll MOD = 100000000LL; // 10^8
const ll BIGMOD = 24LL * MOD; // 2,400,000,000

// Reduce __int128 to [0, m)
ll mmod(lll x, ll m) {
    x %= m;
    if (x < 0) x += m;
    return (ll)x;
}

// P(K) mod MOD using the numerator-mod-BIGMOD trick
ll P_mod(ll K) {
    if (K < 0) return 0;
    lll a = mmod((lll)(K + 1), BIGMOD);
    lll b = mmod((lll)(K + 2), BIGMOD);
    lll c = mmod((lll)K * K - K + 12, BIGMOD);
    ll num = (ll)(a * b % BIGMOD * c % BIGMOD);
    return (num / 24) % MOD;
}

// Prefix power sums mod BIGMOD.
// We use the exact-division trick: compute numerator mod (denom * BIGMOD),
// then integer-divide by denom to get result mod BIGMOD.
ll prefix_pow(ll n, int k) {
    if (n < 0) return 0;
    lll nn = n;
    if (k == 0) {
        return mmod(nn + 1, BIGMOD);
    }
    if (k == 1) {
        // n(n+1)/2
        lll M = (lll)2 * BIGMOD;
        lll val = (lll)mmod(nn, M) * mmod(nn + 1, M) % M;
        return (ll)((val / 2) % BIGMOD);
    }
    if (k == 2) {
        // n(n+1)(2n+1)/6
        lll M = (lll)6 * BIGMOD;
        lll val = (lll)mmod(nn, M) * mmod(nn + 1, M) % M;
        val = val * mmod(2 * nn + 1, M) % M;
        return (ll)((val / 6) % BIGMOD);
    }
    if (k == 3) {
        // [n(n+1)/2]^2
        lll M2 = (lll)2 * BIGMOD;
        lll half = (lll)mmod(nn, M2) * mmod(nn + 1, M2) % M2;
        half = half / 2;
        half %= BIGMOD;
        return (ll)(half * half % BIGMOD);
    }
    if (k == 4) {
        // n(n+1)(2n+1)(3n^2+3n-1)/30
        lll M = (lll)30 * BIGMOD;
        lll a = mmod(nn, M);
        lll b = mmod(nn + 1, M);
        lll c = mmod(2 * nn + 1, M);
        lll d = mmod((lll)3 * nn * nn + 3 * nn - 1, M);
        lll val = a * b % M;
        val = val * c % M;
        val = val * d % M;
        return (ll)((val / 30) % BIGMOD);
    }
    return 0;
}

ll sum_pow_range(ll a, ll b, int k) {
    if (a > b) return 0;
    return (prefix_pow(b, k) - prefix_pow(a - 1, k) + BIGMOD) % BIGMOD;
}

// sum_{v=v1}^{v2} P(coeff*v + offset) mod MOD
//
// P(w) = (w^4 + 2w^3 + 11w^2 + 34w + 24) / 24
// w = c*v + d, where c = coeff, d = offset.
//
// The coefficient of v^j in the numerator of P(cv+d) is:
//   A_j = sum_{deg=j..4} weight[deg] * C(deg,j) * c^j * d^(deg-j)
// where weight = {24, 34, 11, 2, 1} for deg = {0,1,2,3,4}.
//
// We compute A_j mod BIGMOD, then:
//   sum_v P(cv+d) = sum_j A_j * (sum_{v=v1}^{v2} v^j) / 24
// Everything mod BIGMOD, divide by 24 at end.

ll sum_P_linear_mod(ll v1, ll v2, ll coeff, ll offset) {
    if (v1 > v2) return 0;

    // c^j mod BIGMOD for j = 0..4
    ll cpow[5];
    cpow[0] = 1;
    for (int j = 1; j <= 4; j++)
        cpow[j] = mmod((lll)cpow[j-1] * coeff, BIGMOD);

    // d^k mod BIGMOD for k = 0..4
    ll dpow[5];
    dpow[0] = 1;
    for (int j = 1; j <= 4; j++)
        dpow[j] = mmod((lll)dpow[j-1] * offset, BIGMOD);

    // Binomial coefficients (small, precomputed)
    int binom[5][5] = {
        {1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 2, 1, 0, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 6, 4, 1}
    };

    int weight[5] = {24, 34, 11, 2, 1};

    lll total = 0;
    for (int j = 0; j <= 4; j++) {
        // A_j = sum_{deg=j}^{4} weight[deg] * binom[deg][j] * c^j * d^{deg-j}
        lll Aj = 0;
        for (int deg = j; deg <= 4; deg++) {
            lll term = (lll)weight[deg] * binom[deg][j];
            term = term % BIGMOD * cpow[j] % BIGMOD;
            term = term * dpow[deg - j] % BIGMOD;
            Aj = (Aj + term) % BIGMOD;
        }

        ll sp = sum_pow_range(v1, v2, j);
        total = (total + Aj * sp) % BIGMOD;
    }

    return (ll)((total / 24) % MOD);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto S_compute = [&](ll N) -> ll {
        ll total = 0;
        ll sq = (ll)sqrtl((long double)N);
        while ((sq + 1) * (sq + 1) <= N) sq++;
        while (sq * sq > N) sq--;

        // Part 1: u = 0
        total = (total + P_mod(N)) % MOD;

        // Part 2: u = 1 .. sq
        for (ll u = 1; u <= sq; u++) {
            ll s_hi_b = (N - u * u) / u;
            ll s_hi_a = N - 2 * u;
            ll s_hi = min(s_hi_b, s_hi_a);
            if (s_hi >= 0) {
                total = (total + P_mod(s_hi)) % MOD;
            }
        }

        // Part 3: v = 1 .. sq (u = -v)
        for (ll v = 1; v <= sq; v++) {
            ll s_hi = v + N / v;
            ll s_lo = max(0LL, 2 * v - N);
            if (s_hi >= s_lo) {
                total = (total + P_mod(s_hi) - P_mod(s_lo - 1) + MOD) % MOD;
            }
        }

        // Part 4: v = sq+1 .. N, grouped by q = N/v
        ll v = sq + 1;
        while (v <= N) {
            ll q = N / v;
            if (q == 0) break;
            ll v_end = min(N / q, N);
            ll v_cut = N - q;

            ll v_start = v;
            ll v_end_A = min(v_cut, v_end);
            ll v_start_B = max(v_start, v_cut + 1);
            ll v_end_B = v_end;

            if (v_start <= v_end_A) {
                ll add = (sum_P_linear_mod(v_start, v_end_A, 1, q)
                        - sum_P_linear_mod(v_start, v_end_A, 1, -q - 1) + MOD) % MOD;
                total = (total + add) % MOD;
            }

            if (v_start_B <= v_end_B) {
                ll add = (sum_P_linear_mod(v_start_B, v_end_B, 1, q)
                        - sum_P_linear_mod(v_start_B, v_end_B, 2, -N - 1) + MOD) % MOD;
                total = (total + add) % MOD;
            }

            v = v_end + 1;
        }

        return total;
    };

    // Verify
    printf("S(5) mod 10^8 = %lld (expected 344)\n", S_compute(5));
    printf("S(100) mod 10^8 = %lld (expected 26709528)\n", S_compute(100));
    printf("S(1000) mod 10^8 = %lld (expected %lld)\n", S_compute(1000), 263967605900LL % MOD);

    // Main computation
    ll N = 1000000000000LL; // 10^12
    auto t0 = chrono::high_resolution_clock::now();
    ll ans = S_compute(N);
    auto t1 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t1 - t0).count();

    printf("\nS(10^12) mod 10^8 = %lld\n", ans);
    printf("Time: %.3f s\n", elapsed);

    return 0;
}
