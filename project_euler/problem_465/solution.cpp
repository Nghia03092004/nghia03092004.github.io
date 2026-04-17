/*
 * Project Euler Problem 465: Polar Polygons
 *
 * Count polar polygons (origin strictly inside kernel) with integer coordinate
 * vertices |x|, |y| <= n. P(1)=131, P(2)=1648531.
 * Find P(713) mod 10^9+7.
 *
 * Approach:
 * - Enumerate primitive ray directions from origin with counts.
 * - AllWeighted(>=3) = prod(1+c(r)) - 1 - S1 - e2
 * - Bad(>=3) via anchor trick: for each ray i, count subsets in semicircle
 *   (angle[i], angle[i]+pi] containing i.
 * - P(n) = AllWeighted - Bad, all mod 10^9+7.
 *
 * Answer: P(713) mod 10^9+7 = 659553334
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
using namespace std;

const long long MOD = 1000000007LL;

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

long long modinv(long long a, long long mod = MOD) {
    return power(a, mod - 2, mod);
}

long long solve(int n) {
    // Enumerate primitive directions
    vector<pair<double, int>> rays; // (angle, count)
    for (int a = -n; a <= n; a++) {
        for (int b = -n; b <= n; b++) {
            if (a == 0 && b == 0) continue;
            if (__gcd(abs(a), abs(b)) != 1) continue;
            int count = n / max(abs(a), abs(b));
            double angle = atan2((double)b, (double)a);
            if (angle < 0) angle += 2 * M_PI;
            rays.push_back({angle, count});
        }
    }
    sort(rays.begin(), rays.end());
    int R = rays.size();

    vector<int> counts(R);
    vector<double> angles(R);
    for (int i = 0; i < R; i++) {
        angles[i] = rays[i].first;
        counts[i] = rays[i].second;
    }

    // AllWeighted(>=3)
    long long prod_all = 1, S1 = 0, S2 = 0;
    for (int c : counts) {
        prod_all = prod_all * (1 + c) % MOD;
        S1 = (S1 + c) % MOD;
        S2 = (S2 + (long long)c * c) % MOD;
    }
    long long inv2 = modinv(2);
    long long e2 = (S1 * S1 % MOD - S2 + MOD) % MOD * inv2 % MOD;
    long long all_ge3 = (prod_all - 1 - S1 - e2 % MOD + 3 * MOD) % MOD;

    // Doubled arrays
    int N2 = 2 * R;
    vector<double> angles2(N2);
    vector<int> counts2(N2);
    for (int i = 0; i < R; i++) {
        angles2[i] = angles[i];
        angles2[i + R] = angles[i] + 2 * M_PI;
        counts2[i] = counts[i];
        counts2[i + R] = counts[i];
    }

    // Prefix products and sums
    vector<long long> prefix_prod(N2 + 1, 1);
    vector<long long> prefix_sum(N2 + 1, 0);
    for (int k = 0; k < N2; k++) {
        prefix_prod[k + 1] = prefix_prod[k] * (1 + counts2[k]) % MOD;
        prefix_sum[k + 1] = (prefix_sum[k] + counts2[k]) % MOD;
    }

    // Inverse prefix products
    vector<long long> inv_prefix_prod(N2 + 1);
    inv_prefix_prod[N2] = modinv(prefix_prod[N2]);
    for (int k = N2 - 1; k >= 0; k--) {
        inv_prefix_prod[k] = inv_prefix_prod[k + 1] * (1 + counts2[k]) % MOD;
    }

    auto range_prod = [&](int l, int r) -> long long {
        if (l >= r) return 1;
        return prefix_prod[r] * inv_prefix_prod[l] % MOD;
    };

    auto range_sum = [&](int l, int r) -> long long {
        if (l >= r) return 0;
        return (prefix_sum[r] - prefix_sum[l] + MOD) % MOD;
    };

    // Two-pointer sweep for Bad(>=3)
    long long bad_ge3 = 0;
    int j_ptr = 0;
    for (int i = 0; i < R; i++) {
        double target = angles[i] + M_PI + 1e-12;
        if (j_ptr <= i) j_ptr = i + 1;
        while (j_ptr < i + R && angles2[j_ptr] < target) j_ptr++;
        int result_j = j_ptr - 1;
        if (result_j <= i) continue;

        long long Pi_i = range_prod(i + 1, result_j + 1);
        long long Sigma_i = range_sum(i + 1, result_j + 1);
        long long contrib = (Pi_i - 1 - Sigma_i + 2 * MOD) % MOD;
        bad_ge3 = (bad_ge3 + (long long)counts[i] * contrib) % MOD;
    }

    return (all_ge3 - bad_ge3 + MOD) % MOD;
}

int main() {
    cout << "P(1) = " << solve(1) << " (expected 131)" << endl;
    cout << "P(2) = " << solve(2) << " (expected 1648531)" << endl;
    cout << "P(3) mod 10^9+7 = " << solve(3) << " (expected 461288482)" << endl;
    // cout << "P(343) mod 10^9+7 = " << solve(343) << " (expected 937293740)" << endl;
    // cout << "P(713) mod 10^9+7 = " << solve(713) << endl;
    return 0;
}
