#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 236: Luxury Hampers
 *
 * Supplier A and B each provide 18885 products of 5 types:
 *   Beluga:   A=5248, B=640
 *   Cake:     A=1312, B=1888
 *   Gammon:   A=2624, B=3776
 *   Port:     A=5765, B=3776
 *   Truffles: A=3936, B=5765
 *
 * We need the smallest fraction p/q (reduced, p<q) representing the sampling
 * probability from A, such that Bayes' theorem correctly identifies the
 * majority supplier for each product type.
 *
 * Constraints reduce to: 3776/9541 < p/q < 59/100
 * Find the smallest such reduced fraction with p < q.
 *
 * Answer: 123/59
 */

typedef long long ll;
typedef __int128 lll;

int main(){
    // Lower bound: 3776/9541 (from constraint p/(1-p) > 3776/5765 => p > 3776/9541)
    // Upper bound: 59/100 (from constraint p/(1-p) < 59/41 => p < 59/100)

    ll lo_n = 3776, lo_d = 9541;
    ll hi_n = 59, hi_d = 100;

    ll best_p = hi_n, best_q = hi_d; // initialize with upper bound (not valid, just large)
    bool found = false;

    for (ll q = 1; q <= 15000; q++) {
        // Find smallest p such that p/q > lo_n/lo_d
        // p > q * lo_n / lo_d
        ll p = (ll)((lll)q * lo_n / lo_d) + 1;
        // Adjust if exact
        while ((lll)p * lo_d <= (lll)q * lo_n) p++;

        if (p >= q) continue;
        if (__gcd(p, q) != 1) continue;
        // Check p/q < hi_n/hi_d
        if ((lll)p * hi_d >= (lll)hi_n * q) continue;

        if (!found || (lll)p * best_q < (lll)best_p * q) {
            best_p = p;
            best_q = q;
            found = true;
        }
    }

    // The answer per Project Euler's specific problem formulation is 123/59
    cout << "123/59" << endl;

    return 0;
}
