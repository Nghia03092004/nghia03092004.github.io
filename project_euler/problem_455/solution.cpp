/**
 * Project Euler Problem 455: Powers With Trailing Digits
 *
 * f(n) = largest x in [1, 10^9) with n^x = x (mod 10^9), or 0 if none.
 * Find sum f(n) for 2 <= n <= 10^6.
 *
 * Algorithm: CRT decomposition mod 2^9 and mod 5^9.
 * For each prime power p^k with gcd(n,p)=1, solve n^x = x (mod p^k)
 * via Hensel-style lifting over M_j = lcm(p^j, lambda(p^j)).
 * When p | n, the only residue class is x = 0 (mod p^k).
 * Combine via CRT and verify.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 * Run: ./solution [N_max]  (default 10^6)
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

using namespace std;
typedef long long ll;
typedef __int128 lll;

const ll MOD9 = 1000000000LL;
const ll PK2 = 512;         // 2^9
const ll PK5 = 1953125;     // 5^9

ll power_mod(ll base, ll exp, ll m) {
    if (m == 1) return 0;
    ll result = 1;
    base %= m;
    if (base < 0) base += m;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % m;
        exp >>= 1;
        if (exp > 0) base = (lll)base * base % m;
    }
    return result;
}

ll gcd_func(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

ll lcm_func(ll a, ll b) {
    return a / gcd_func(a, b) * b;
}

ll ext_gcd(ll a, ll b, ll &u, ll &v) {
    if (b == 0) { u = 1; v = 0; return a; }
    ll u1, v1;
    ll g = ext_gcd(b, a % b, u1, v1);
    u = v1;
    v = u1 - (a / b) * v1;
    return g;
}

/**
 * Carmichael function lambda(p^j).
 */
ll carmichael(ll p, int j) {
    if (j == 0) return 1;
    if (p == 2) {
        if (j == 1) return 1;
        if (j == 2) return 2;
        ll r = 1;
        for (int i = 0; i < j - 2; i++) r *= 2;
        return r;
    }
    ll r = p - 1;
    for (int i = 1; i < j; i++) r *= p;
    return r;
}

/**
 * M_j = lcm(p^j, lambda(p^j))
 */
ll combined_mod(ll p, int j) {
    if (j == 0) return 1;
    ll pj = 1;
    for (int i = 0; i < j; i++) pj *= p;
    return lcm_func(pj, carmichael(p, j));
}

/**
 * Solve n^x = x (mod p^k) via lifting.
 * Returns list of residues mod p^k.
 */
vector<ll> solve_mod_pk(ll n, ll p, int k) {
    ll pk = 1;
    for (int i = 0; i < k; i++) pk *= p;

    if (n % p == 0) {
        return {0};
    }

    // Base: level 1
    ll M1 = combined_mod(p, 1);
    vector<ll> sols;
    for (ll x = 0; x < M1; x++) {
        if (power_mod(n, x, p) == x % p) {
            sols.push_back(x);
        }
    }

    // Lift levels 1..k-1
    for (int j = 1; j < k; j++) {
        ll Mj = combined_mod(p, j);
        ll Mj1 = combined_mod(p, j + 1);
        ll branch = Mj1 / Mj;
        ll pj1 = 1;
        for (int i = 0; i <= j; i++) pj1 *= p;

        vector<ll> new_sols;
        for (ll xj : sols) {
            for (ll t = 0; t < branch; t++) {
                ll x_cand = xj + t * Mj;
                if (power_mod(n, x_cand, pj1) == x_cand % pj1) {
                    new_sols.push_back(x_cand);
                }
            }
        }
        sols = new_sols;
        if (sols.empty()) return {};
    }

    return sols;
}

ll compute_f(ll n) {
    if (n % 10 == 0) return 0;

    vector<ll> sols2_raw = solve_mod_pk(n, 2, 9);
    if (sols2_raw.empty()) return 0;

    vector<ll> sols5_raw = solve_mod_pk(n, 5, 9);
    if (sols5_raw.empty()) return 0;

    // Deduplicate mod p^k
    set<ll> s2_set, s5_set;
    for (ll s : sols2_raw) s2_set.insert(s % PK2);
    for (ll s : sols5_raw) s5_set.insert(s % PK5);

    ll best = 0;
    for (ll r2 : s2_set) {
        for (ll r5 : s5_set) {
            ll u, v;
            ll g = ext_gcd(PK2, PK5, u, v);
            if ((r5 - r2) % g != 0) continue;
            ll lcm_val = PK2 / g * PK5;
            ll diff = (r5 - r2) / g;
            // x = r2 + PK2 * (diff * u) mod (PK5/g)
            ll mod2 = PK5 / g;
            ll step = ((lll)diff * u % mod2 + mod2) % mod2;
            ll x = r2 + PK2 * step;
            x = ((x % lcm_val) + lcm_val) % lcm_val;

            if (x > 0 && x < MOD9) {
                if (power_mod(n, x, MOD9) == x) {
                    if (x > best) best = x;
                }
            }
        }
    }

    return best;
}

int main(int argc, char* argv[]) {
    ll N_max = 1000000;
    if (argc > 1) N_max = atoll(argv[1]);

    printf("Problem 455: Powers With Trailing Digits\n");
    printf("Computing sum f(n) for 2 <= n <= %lld...\n", (long long)N_max);

    // Checkpoints
    ll f4 = compute_f(4);
    printf("  f(4) = %lld (expected 411728896) %s\n",
           (long long)f4, f4 == 411728896LL ? "PASS" : "FAIL");
    ll f157 = compute_f(157);
    printf("  f(157) = %lld (expected 743757) %s\n",
           (long long)f157, f157 == 743757LL ? "PASS" : "FAIL");
    ll f10 = compute_f(10);
    printf("  f(10) = %lld (expected 0) %s\n",
           (long long)f10, f10 == 0 ? "PASS" : "FAIL");

    if (N_max >= 1000) {
        ll ck = 0;
        for (ll n = 2; n <= 1000; n++) ck += compute_f(n);
        printf("  sum f(2..1000) = %lld (expected 442530011399) %s\n",
               (long long)ck, ck == 442530011399LL ? "PASS" : "FAIL");
    }

    clock_t t_start = clock();
    ll total = 0;
    int nz = 0;

    for (ll n = 2; n <= N_max; n++) {
        ll fn = compute_f(n);
        total += fn;
        if (fn > 0) nz++;
        if (n % 100000 == 0) {
            double el = (double)(clock() - t_start) / CLOCKS_PER_SEC;
            printf("  n=%lld sum=%lld nz=%d t=%.1fs\n",
                   (long long)n, (long long)total, nz, el);
        }
    }

    double elapsed = (double)(clock() - t_start) / CLOCKS_PER_SEC;
    printf("\nResult: sum f(n) for 2 <= n <= %lld = %lld\n",
           (long long)N_max, (long long)total);
    printf("Nonzero count: %d\n", nz);
    printf("Time: %.3f seconds\n", elapsed);

    if (N_max == 1000000LL) {
        printf("Expected: 450186511399999 -> %s\n",
               total == 450186511399999LL ? "PASS" : "FAIL");
    }

    return 0;
}
