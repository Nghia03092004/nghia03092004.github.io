#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 402: Integer-valued Polynomials
 *
 * M(a,b,c) = gcd(24, 36+6a, 14+6a+2b, 1+a+b+c)
 * S(N) = sum_{0<a,b,c<=N} M(a,b,c)
 * Find last 9 digits of sum_{k=2}^{1234567890123} S(F_k)
 *
 * Answer: 356019862
 */

typedef long long ll;
typedef __int128 lll;
typedef vector<vector<ll>> Matrix;

const ll MOD_FINAL = 1000000000LL;       // 10^9
const ll WORK_MOD  = 288LL * MOD_FINAL;  // 288 * 10^9

// =====================================================================
// Safe modular reduction for __int128
// =====================================================================

inline ll mmod(lll x) {
    ll r = (ll)(x % WORK_MOD);
    return r < 0 ? r + WORK_MOD : r;
}

// =====================================================================
// Matrix operations mod WORK_MOD
// =====================================================================

Matrix mat_mul(const Matrix& A, const Matrix& B) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Matrix C(n, vector<ll>(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            lll s = 0;
            for (int l = 0; l < k; l++)
                s += (lll)A[i][l] * B[l][j];
            C[i][j] = mmod(s);
        }
    return C;
}

Matrix mat_pow(Matrix A, ll p) {
    int n = A.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = mat_mul(result, A);
        A = mat_mul(A, A);
        p >>= 1;
    }
    return result;
}

vector<ll> mat_vec(const Matrix& A, const vector<ll>& v) {
    int n = A.size();
    vector<ll> result(n, 0);
    for (int i = 0; i < n; i++) {
        lll s = 0;
        for (int j = 0; j < (int)v.size(); j++)
            s += (lll)A[i][j] * v[j];
        result[i] = mmod(s);
    }
    return result;
}

// =====================================================================
// S(N) analytical computation (for verification)
// =====================================================================

int euler_totient(int n) {
    int result = n, temp = n;
    for (int p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

ll S_analytical(ll N) {
    int divs[] = {1, 2, 3, 4, 6, 8, 12, 24};
    ll total = 0;
    for (int d : divs) {
        int phi_d = euler_totient(d);
        ll cnt = 0;
        for (int ar = 0; ar < d; ar++) {
            if ((36 + 6 * ar) % d != 0) continue;
            int first_a = (ar >= 1) ? ar : d;
            if (first_a > N) continue;
            ll cnt_a = (N - first_a) / d + 1;
            for (int br = 0; br < d; br++) {
                if ((14 + 6 * ar + 2 * br) % d != 0) continue;
                int first_b = (br >= 1) ? br : d;
                if (first_b > N) continue;
                ll cnt_b = (N - first_b) / d + 1;
                int cr = ((-(1 + ar + br)) % d + d) % d;
                int first_c = (cr >= 1) ? cr : d;
                if (first_c > N) continue;
                ll cnt_c = (N - first_c) / d + 1;
                cnt += cnt_a * cnt_b * cnt_c;
            }
        }
        total += (ll)phi_d * cnt;
    }
    return total;
}

// =====================================================================
// Piecewise-cubic polynomial coefficients for 288 * S(N)
// 288*S(N) = A*N^3 + B*N^2 + C*N + D when N ≡ s (mod 24)
// =====================================================================

struct Poly { ll D, C, B, A; };

Poly POLYS[24] = {
    {0, 0, 0, 583},           // s=0
    {257, -75, -189, 583},    // s=1
    {520, 84, 102, 583},      // s=2
    {1215, -27, -51, 583},    // s=3
    {256, -64, -136, 583},    // s=4
    {2313, 229, 11, 583},     // s=5
    {360, -108, 14, 583},     // s=6
    {1063, -139, -139, 583},  // s=7
    {512, 128, 64, 583},      // s=8
    {225, 21, -125, 583},     // s=9
    {-1464, -140, -122, 583}, // s=10
    {4487, 245, 85, 583},     // s=11
    {4032, 0, 0, 583},        // s=12
    {4289, -75, -189, 583},   // s=13
    {1672, 84, 102, 583},     // s=14
    {1791, -27, -51, 583},    // s=15
    {832, -64, -136, 583},    // s=16
    {2889, 229, 11, 583},     // s=17
    {360, -108, 14, 583},     // s=18
    {1639, -139, -139, 583},  // s=19
    {1088, 128, 64, 583},     // s=20
    {801, 21, -125, 583},     // s=21
    {-1464, -140, -122, 583}, // s=22
    {455, 245, 85, 583}       // s=23
};

// Fibonacci mod 24 table (Pisano period = 24)
int fib_mod24[25];
void init_fib_mod24() {
    fib_mod24[0] = 0; fib_mod24[1] = 1;
    for (int i = 2; i < 25; i++)
        fib_mod24[i] = (fib_mod24[i-1] + fib_mod24[i-2]) % 24;
}

// =====================================================================
// Build 14x14 transition matrix
// State: [u^3, u^2v, uv^2, v^3, u^2, uv, v^2, u, v, 1,
//         sum_u^3, sum_u^2, sum_u, sum_1]
// Transition: u' = a*u + b*v, v' = c*u + d*v
// =====================================================================

Matrix build_transition(ll a, ll b, ll c, ll d) {
    Matrix T(14, vector<ll>(14, 0));

    // Degree-3 monomials
    T[0][0] = mmod((lll)a*a*a);
    T[0][1] = mmod((lll)3*a*a*b);
    T[0][2] = mmod((lll)3*a*b*b);
    T[0][3] = mmod((lll)b*b*b);

    T[1][0] = mmod((lll)a*a*c);
    T[1][1] = mmod((lll)a*a*d + (lll)2*a*b*c);
    T[1][2] = mmod((lll)2*a*b*d + (lll)b*b*c);
    T[1][3] = mmod((lll)b*b*d);

    T[2][0] = mmod((lll)a*c*c);
    T[2][1] = mmod((lll)2*a*c*d + (lll)b*c*c);
    T[2][2] = mmod((lll)a*d*d + (lll)2*b*c*d);
    T[2][3] = mmod((lll)b*d*d);

    T[3][0] = mmod((lll)c*c*c);
    T[3][1] = mmod((lll)3*c*c*d);
    T[3][2] = mmod((lll)3*c*d*d);
    T[3][3] = mmod((lll)d*d*d);

    // Degree-2 monomials
    T[4][4] = mmod((lll)a*a); T[4][5] = mmod((lll)2*a*b); T[4][6] = mmod((lll)b*b);
    T[5][4] = mmod((lll)a*c); T[5][5] = mmod((lll)a*d+(lll)b*c); T[5][6] = mmod((lll)b*d);
    T[6][4] = mmod((lll)c*c); T[6][5] = mmod((lll)2*c*d); T[6][6] = mmod((lll)d*d);

    // Degree-1
    T[7][7] = a; T[7][8] = b;
    T[8][7] = c; T[8][8] = d;

    // Constant
    T[9][9] = 1;

    // Accumulators: Su3' = Su3 + u'^3, etc.
    for (int j = 0; j < 4; j++) T[10][j] = T[0][j];
    T[10][10] = 1;

    for (int j = 4; j < 7; j++) T[11][j] = T[4][j];
    T[11][11] = 1;

    T[12][7] = T[7][7]; T[12][8] = T[7][8];
    T[12][12] = 1;

    T[13][9] = 1; T[13][13] = 1;

    return T;
}

// =====================================================================
// Main solver
// =====================================================================

ll solve(ll K) {
    init_fib_mod24();

    // Fibonacci constants: F_23=28657, F_24=46368, F_25=75025
    // Transition: u' = F23*u + F24*v, v' = F24*u + F25*v
    const ll F23 = 28657, F24 = 46368, F25 = 75025;

    Matrix T = build_transition(F23, F24, F24, F25);

    // Small Fibonacci values for initial conditions
    ll fib_small[30];
    fib_small[0] = 0; fib_small[1] = 1;
    for (int i = 2; i < 30; i++)
        fib_small[i] = fib_small[i-1] + fib_small[i-2];

    ll total_288 = 0;

    for (int r = 0; r < 24; r++) {
        int s = fib_mod24[r];
        Poly& poly = POLYS[s];

        // Polynomial coefficients mod WORK_MOD (handle negative values)
        ll D_c = mmod(poly.D);
        ll C_c = mmod(poly.C);
        ll B_c = mmod(poly.B);
        ll A_c = mmod(poly.A);

        // Index range for k = 24*i + r, with k in [2, K]
        ll i_min = (r >= 2) ? 0 : 1;

        // IMPORTANT: guard against negative (K - r) before division
        if (K < r) continue;
        ll i_max = (K - r) / 24;

        if (i_max < i_min) continue;
        ll n_terms = i_max - i_min + 1;

        // Initial Fibonacci values at k_start = 24*i_min + r
        ll k_start = 24 * i_min + r;
        ll u = fib_small[k_start] % WORK_MOD;
        ll v = fib_small[k_start + 1] % WORK_MOD;

        // Build initial state (with first term already in accumulators)
        vector<ll> state(14);
        state[0]  = mmod((lll)u*u*u);
        state[1]  = mmod((lll)u*u*v);
        state[2]  = mmod((lll)u*v*v);
        state[3]  = mmod((lll)v*v*v);
        state[4]  = mmod((lll)u*u);
        state[5]  = mmod((lll)u*v);
        state[6]  = mmod((lll)v*v);
        state[7]  = u;
        state[8]  = v;
        state[9]  = 1;
        state[10] = state[0]; // Su3 initialized with first term
        state[11] = state[4]; // Su2
        state[12] = u;        // Su
        state[13] = 1;        // count

        if (n_terms > 1) {
            Matrix T_pow = mat_pow(T, n_terms - 1);
            state = mat_vec(T_pow, state);
        }

        ll Su3 = state[10], Su2 = state[11], Su1 = state[12], S1 = state[13];

        lll contrib = (lll)A_c * Su3 + (lll)B_c * Su2
                    + (lll)C_c * Su1 + (lll)D_c * S1;
        total_288 = (total_288 + mmod(contrib)) % WORK_MOD;
    }

    return total_288 / 288;
}

// =====================================================================
// Verification
// =====================================================================

void verify() {
    printf("Verification:\n");

    // M(4,2,5) = 6
    auto M = [](int a, int b, int c) -> int {
        return __gcd(__gcd(24, 36+6*a), __gcd(14+6*a+2*b, 1+a+b+c));
    };
    assert(M(4,2,5) == 6);
    printf("  [PASS] M(4,2,5) = 6\n");

    // S(10) = 1972
    assert(S_analytical(10) == 1972);
    printf("  [PASS] S(10) = 1972\n");

    // S(10000) = 2024258331114
    assert(S_analytical(10000) == 2024258331114LL);
    printf("  [PASS] S(10000) = 2024258331114\n");

    // Cross-validate K=20 with brute force
    ll fib[21]; fib[0] = 0; fib[1] = 1;
    for (int i = 2; i <= 20; i++) fib[i] = fib[i-1] + fib[i-2];
    ll bf_sum = 0;
    for (int k = 2; k <= 20; k++)
        bf_sum = (bf_sum + S_analytical(fib[k]) % MOD_FINAL) % MOD_FINAL;
    ll mat_sum = solve(20);
    printf("  K=20 brute-force: %lld, matrix: %lld\n", bf_sum, mat_sum);
    assert(mat_sum == bf_sum);
    printf("  [PASS] K=20 cross-validation\n");
}

// =====================================================================
// Main
// =====================================================================

int main() {
    printf("Project Euler Problem 402: Integer-valued Polynomials\n");
    printf("=====================================================\n\n");

    verify();
    printf("\n");

    auto t0 = chrono::steady_clock::now();
    ll answer = solve(1234567890123LL);
    auto t1 = chrono::steady_clock::now();
    double elapsed = chrono::duration<double>(t1 - t0).count();

    printf("Answer: %lld\n", answer);
    printf("Time:   %.3f s\n", elapsed);

    return 0;
}
