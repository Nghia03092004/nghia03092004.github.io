/*
 * Project Euler Problem 405: A Rectangular Tiling
 *
 * A rectangle (length = 2 * width) is tiled recursively via substitution.
 * f(n) = number of interior points where exactly 4 tiles meet in T(n).
 *
 * Recurrence:  f(n) = 5*f(n-1) - 2*f(n-2) - 8*f(n-3) + 6
 * Closed form: f(n) = (6*4^n - 20*2^n - (-1)^n + 15) / 15
 *
 * Task: f(10^(10^18)) mod 17^7
 * Answer: 237696125
 *
 * Approach: closed-form modular arithmetic with Euler's theorem
 * for exponent reduction.
 *
 * Compile: g++ -O2 -std=c++17 -o solution solution.cpp
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<vector<ll>> Matrix;

// -----------------------------------------------------------------------
// Modular arithmetic helpers
// -----------------------------------------------------------------------

ll mod(ll a, ll m) {
    return ((a % m) + m) % m;
}

ll power(ll base, ll exp, ll m) {
    ll result = 1;
    base = mod(base, m);
    while (exp > 0) {
        if (exp & 1) result = result * base % m;
        base = base * base % m;
        exp >>= 1;
    }
    return result;
}

ll mod_inv(ll a, ll m) {
    return power(a, m - 2, m);  // Works when m is prime; for prime power, use extended gcd
}

// Extended GCD for modular inverse when m is not prime
ll ext_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll mod_inv_general(ll a, ll m) {
    ll x, y;
    ll g = ext_gcd(a % m, m, x, y);
    assert(g == 1);
    return mod(x, m);
}

// -----------------------------------------------------------------------
// 4x4 matrix operations modulo m
// -----------------------------------------------------------------------

Matrix mat_mul(const Matrix &A, const Matrix &B, ll m) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) if (A[i][k])
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % m;
    return C;
}

Matrix mat_pow(Matrix M, ll p, ll m) {
    int n = M.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = mat_mul(result, M, m);
        M = mat_mul(M, M, m);
        p >>= 1;
    }
    return result;
}

// -----------------------------------------------------------------------
// f(n) mod m via matrix exponentiation
// -----------------------------------------------------------------------

ll f_mod_matrix(ll n, ll m) {
    if (n <= 1) return 0;
    if (n == 2) return 2 % m;
    if (n == 3) return 16 % m;
    Matrix M = {
        {mod(5, m), mod(-2, m), mod(-8, m), mod(6, m)},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1}
    };
    Matrix Mp = mat_pow(M, n - 3, m);
    vector<ll> v = {16, 2, 0, 1};
    ll ans = 0;
    for (int j = 0; j < 4; j++)
        ans = (ans + Mp[0][j] * v[j]) % m;
    return mod(ans, m);
}

// -----------------------------------------------------------------------
// Solve PE 405: f(10^(10^18)) mod 17^7
// -----------------------------------------------------------------------

ll solve() {
    const ll MOD = 1;
    // 17^7
    ll mod17_7 = 1;
    for (int i = 0; i < 7; i++) mod17_7 *= 17;  // 410338673

    // phi(17^7) = 16 * 17^6
    ll mod17_6 = mod17_7 / 17;  // 24137569
    ll phi_17_7 = 16 * mod17_6; // 386201104

    // phi(17^6) = 16 * 17^5
    ll mod17_5 = mod17_6 / 17;  // 1419857
    ll phi_17_6 = 16 * mod17_5; // 22717712

    // N = 10^(10^18).  We need N mod phi(17^7).
    // phi(17^7) = 2^4 * 17^6.
    //
    // mod 16: 10^(10^18) mod 16 = 0  (since 10^k mod 16 = 0 for k >= 4)
    ll r_16 = 0;

    // mod 17^6: 10^(10^18) mod 17^6
    //         = 10^(10^18 mod phi(17^6)) mod 17^6
    //         10^18 mod phi(17^6):
    ll exp_inner = 1;
    {
        // Compute 10^18 mod phi_17_6 = 10^18 mod 22717712
        // 10^18 fits in unsigned 64-bit, just do it directly
        ll val = 1;
        for (int i = 0; i < 18; i++) {
            val = val * 10 % phi_17_6;
        }
        exp_inner = val;
    }
    ll r_17_6 = power(10, exp_inner, mod17_6);

    // CRT: x = 0 mod 16, x = r_17_6 mod 17^6
    ll inv16 = mod_inv_general(16, mod17_6);
    ll t = r_17_6 * inv16 % mod17_6;
    ll N_mod_phi = 16 * t % phi_17_7;

    // Compute f(N) using closed form:
    // f(N) = (6*4^N - 20*2^N - (-1)^N + 15) * 15^{-1}  mod 17^7
    // N is even => (-1)^N = 1

    ll pow2N = power(2, N_mod_phi, mod17_7);
    ll twoN_mod_phi = 2 * N_mod_phi % phi_17_7;
    ll pow4N = power(2, twoN_mod_phi, mod17_7);

    ll inv15 = mod_inv_general(15, mod17_7);
    ll numerator = mod(6LL * pow4N - 20LL * pow2N - 1 + 15, mod17_7);
    ll answer = numerator % mod17_7 * inv15 % mod17_7;

    return answer;
}

// -----------------------------------------------------------------------
// Verification helpers
// -----------------------------------------------------------------------

void verify_small_values() {
    cout << "--- Small values via recurrence ---\n";
    vector<ll> f = {0, 0, 2, 16};  // f[0]..f[3]
    for (int n = 4; n <= 10; n++) {
        ll fn = 5 * f[n-1] - 2 * f[n-2] - 8 * f[n-3] + 6;
        f.push_back(fn);
    }
    for (int n = 0; n <= 10; n++) {
        cout << "  f(" << n << ") = " << f[n] << "\n";
    }
}

void verify_closed_form() {
    cout << "\n--- Closed form check ---\n";
    for (int n = 1; n <= 10; n++) {
        // f(n) = (6*4^n - 20*2^n - (-1)^n + 15) / 15
        ll val = 6;
        for (int i = 0; i < n; i++) val *= 4;
        ll p2 = 1;
        for (int i = 0; i < n; i++) p2 *= 2;
        val -= 20 * p2;
        val -= (n % 2 == 0 ? 1 : -1);
        val += 15;
        val /= 15;
        cout << "  f(" << n << ") = " << val << "\n";
    }
}

void verify_f_10_9() {
    ll mod17_7 = 1;
    for (int i = 0; i < 7; i++) mod17_7 *= 17;

    ll result = f_mod_matrix(1000000000LL, mod17_7);
    cout << "\n--- Modular check ---\n";
    cout << "  f(10^9) mod 17^7 = " << result;
    cout << (result == 126897180 ? "  [OK]" : "  [FAIL]") << "\n";
}

// -----------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << string(60, '=') << "\n";
    cout << "Project Euler Problem 405: A Rectangular Tiling\n";
    cout << string(60, '=') << "\n\n";

    verify_small_values();
    verify_closed_form();
    verify_f_10_9();

    ll answer = solve();
    cout << "\n" << string(60, '=') << "\n";
    cout << "  ANSWER: f(10^(10^18)) mod 17^7 = " << answer << "\n";
    cout << string(60, '=') << "\n";

    return 0;
}
