#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

/*
 * Problem 103: Special Subset Sums: Optimum
 *
 * Search for the optimal n=7 special sum set by perturbing the
 * near-optimum {20,31,38,39,40,42,45} by +/-3 on each element.
 */

bool is_special(vector<int>& a) {
    int n = a.size();
    sort(a.begin(), a.end());
    // Property 2
    for (int k = 1; k <= n / 2; k++) {
        int lo = 0, hi = 0;
        for (int i = 0; i <= k; i++) lo += a[i];
        for (int i = 0; i < k; i++) hi += a[n - 1 - i];
        if (lo <= hi) return false;
    }
    // Property 1
    set<int> sums;
    for (int mask = 1; mask < (1 << n); mask++) {
        int s = 0;
        for (int i = 0; i < n; i++)
            if (mask & (1 << i)) s += a[i];
        if (sums.count(s)) return false;
        sums.insert(s);
    }
    return true;
}

int main() {
    int base[] = {20, 31, 38, 39, 40, 42, 45};
    int best_sum = 255;
    vector<int> best_set(base, base + 7);
    const int D = 3;

    for (int d0 = -D; d0 <= D; d0++)
    for (int d1 = -D; d1 <= D; d1++)
    for (int d2 = -D; d2 <= D; d2++)
    for (int d3 = -D; d3 <= D; d3++)
    for (int d4 = -D; d4 <= D; d4++)
    for (int d5 = -D; d5 <= D; d5++)
    for (int d6 = -D; d6 <= D; d6++) {
        vector<int> a = {
            base[0]+d0, base[1]+d1, base[2]+d2, base[3]+d3,
            base[4]+d4, base[5]+d5, base[6]+d6
        };
        bool ok = true;
        for (int x : a) if (x <= 0) { ok = false; break; }
        if (!ok) continue;

        sort(a.begin(), a.end());
        for (int i = 1; i < 7; i++)
            if (a[i] <= a[i-1]) { ok = false; break; }
        if (!ok) continue;

        int s = 0;
        for (int x : a) s += x;
        if (s > best_sum) continue;

        if (is_special(a)) {
            if (s < best_sum || (s == best_sum && a < best_set)) {
                best_sum = s;
                best_set = a;
            }
        }
    }

    sort(best_set.begin(), best_set.end());
    string ans;
    for (int x : best_set) ans += to_string(x);
    cout << ans << endl;
    return 0;
}
