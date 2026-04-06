/**
 * Project Euler Problem 454: Diophantine Reciprocals III
 *
 * In 1/x + 1/y = 1/n (positive integers, x < y), F(L) counts solutions
 * with y <= L. Find F(10^12).
 *
 * Key identity:
 *   F(L) = sum over coprime pairs (u,v) with 1<=u<v
 *          of floor(L / (v*(u+v)))
 *
 * Derivation: x = k*u*(u+v), y = k*v*(u+v), n = k*u*v with gcd(u,v)=1.
 *
 * For each v, we iterate u from 1 to v-1 checking gcd(u,v)=1.
 * v ranges up to sqrt(L) ~ 10^6 for L = 10^12.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 * Run: ./solution [L]  (default L = 10^12)
 */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;

// Fast GCD
inline ll gcd(ll a, ll b) {
    while (b) { a %= b; ll t = a; a = b; b = t; }
    return a;
}

int main(int argc, char* argv[]) {
    ll L = 1000000000000LL; // 10^12
    if (argc > 1) {
        L = atoll(argv[1]);
    }

    printf("Problem 454: Diophantine Reciprocals III\n");
    printf("Computing F(%lld)...\n", (long long)L);

    clock_t t_start = clock();

    ll v_max = (ll)sqrt((double)L);
    while (v_max * (v_max + 1) > L) v_max--;
    while ((v_max + 1) * (v_max + 2) <= L) v_max++;
    // v_max is the largest v such that v*(v+1) <= L (since u >= 1 means u+v >= v+1)

    printf("v_max = %lld\n", (long long)v_max);

    ll result = 0;

    for (ll v = 2; v <= v_max; v++) {
        for (ll u = 1; u < v; u++) {
            ll denom = v * (u + v);
            if (denom > L) break;
            if (gcd(u, v) == 1) {
                result += L / denom;
            }
        }
        if (v % 100000 == 0) {
            double elapsed = (double)(clock() - t_start) / CLOCKS_PER_SEC;
            printf("  v = %lld / %lld, partial = %lld, time = %.1fs\n",
                   (long long)v, (long long)v_max, (long long)result, elapsed);
        }
    }

    double elapsed = (double)(clock() - t_start) / CLOCKS_PER_SEC;

    printf("\nResult:\n");
    printf("  F(%lld) = %lld\n", (long long)L, (long long)result);
    printf("  Time: %.3f seconds\n", elapsed);

    // Verification
    if (L == 15LL) {
        printf("  Expected: 4 -> %s\n", result == 4 ? "PASS" : "FAIL");
    }
    if (L == 1000LL) {
        printf("  Expected: 1069 -> %s\n", result == 1069 ? "PASS" : "FAIL");
    }
    if (L == 1000000000000LL) {
        printf("  Expected: 5435004633092 -> %s\n",
               result == 5435004633092LL ? "PASS" : "FAIL");
    }

    return 0;
}
