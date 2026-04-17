#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 397 -- Triangle on Parabola
 *
 * On parabola y = x^2/k, points with integer x-coords a < b < c
 * (0 < a < b < c <= k) form a triangle with area = (b-a)(c-b)(c-a)/(2k).
 *
 * S(k) = # triples with area <= k, i.e. (b-a)(c-b)(c-a) <= 2k^2.
 *
 * Substituting u = b-a, v = c-b:
 *   u*v*(u+v) <= 2k^2, u >= 1, v >= 1, u+v <= k-1
 *   Each valid (u,v) contributes (k - u - v) triples.
 *
 * For each u, find max v via quadratic formula on u*v^2 + u^2*v <= 2k^2.
 */

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// Find max v >= 1 such that u*v*(u+v) <= bound
ll max_v_for_u(ll u, ll bound) {
    // Check if v=1 works
    if (u * 1LL * (u + 1) > bound) return 0;

    // u*v^2 + u^2*v <= bound
    // v^2 + u*v - bound/u <= 0
    // v <= (-u + sqrt(u^2 + 4*bound/u)) / 2
    double disc = (double)u * u + 4.0 * (double)bound / u;
    ll v_max = (ll)((-u + sqrt(disc)) / 2.0);

    // Adjust for floating point errors using 128-bit arithmetic
    while (v_max >= 1 && (lll)u * v_max * (u + v_max) > bound) v_max--;
    while ((lll)u * (v_max + 1) * (u + v_max + 1) <= bound) v_max++;

    return max(v_max, 0LL);
}

ll S(ll k) {
    ll bound = 2LL * k * k;
    ll total = 0;

    for (ll u = 1; u <= k - 2; u++) {
        ll vm_area = max_v_for_u(u, bound);
        if (vm_area < 1) break;  // monotone: larger u won't work either

        ll vm_range = k - 1 - u;
        if (vm_range < 1) break;

        ll vm = min(vm_area, vm_range);

        // Sum_{v=1}^{vm} (k - u - v) = vm*(k-u) - vm*(vm+1)/2
        total += vm * (k - u) - vm * (vm + 1) / 2;
    }

    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Verify small cases
    cout << "Small cases:" << endl;
    for (int k = 3; k <= 20; k++) {
        cout << "  S(" << k << ") = " << S(k) << endl;
    }

    // Compute moderate cases
    cout << endl;
    for (ll k : {100LL, 1000LL, 10000LL, 100000LL}) {
        auto t0 = chrono::high_resolution_clock::now();
        ll result = S(k);
        auto t1 = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();
        cout << "S(" << k << ") = " << result << "  (" << ms << " ms)" << endl;
    }

    // Compute answer
    cout << endl << "Computing S(10^6)..." << endl;
    auto t0 = chrono::high_resolution_clock::now();
    ll answer = S(1000000LL);
    auto t1 = chrono::high_resolution_clock::now();
    double sec = chrono::duration<double>(t1 - t0).count();

    cout << "S(10^6) = " << answer << "  (" << sec << " s)" << endl;
    cout << endl << "Answer: " << answer << endl;

    return 0;
}
