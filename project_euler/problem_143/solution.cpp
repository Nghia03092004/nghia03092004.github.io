#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 143: Torricelli point distances
    // Find sum of all distinct p+q+r <= 120000
    // where all three pair-wise expressions u^2+uv+v^2 are perfect squares.
    //
    // Primitive 120-compatible pairs: u = m^2-n^2, v = 2mn+n^2
    // with m > n >= 1, gcd(m,n) = 1, (m-n) % 3 != 0.
    // Then u^2+uv+v^2 = (m^2+mn+n^2)^2.
    // All solutions are k*(u,v) for k >= 1.

    const int LIMIT = 120000;

    // smaller[u] = sorted vector of v < u compatible with u
    vector<vector<int>> smaller(LIMIT + 1);

    auto addPrimitive = [&](int a, int b) {
        if (a <= 0 || b <= 0) return;
        int u = max(a, b), v = min(a, b);
        for (int k = 1; (long long)k * (u + v) <= LIMIT; k++) {
            smaller[k * u].push_back(k * v);
        }
    };

    for (int m = 2; (long long)m * m < LIMIT; m++) {
        for (int n = 1; n < m; n++) {
            if (__gcd(m, n) != 1) continue;
            if ((m - n) % 3 == 0) continue;
            int a = m * m - n * n;
            int b = 2 * m * n + n * n;
            addPrimitive(a, b);
        }
    }

    // Sort and deduplicate each adjacency list
    for (int i = 0; i <= LIMIT; i++) {
        sort(smaller[i].begin(), smaller[i].end());
        smaller[i].erase(unique(smaller[i].begin(), smaller[i].end()), smaller[i].end());
    }

    // Find triangles
    set<int> valid_sums;

    for (int p = 1; p <= LIMIT; p++) {
        if (smaller[p].empty()) continue;
        // For quick lookup, use a set for smaller[p]
        unordered_set<int> sp_set(smaller[p].begin(), smaller[p].end());

        for (int q : smaller[p]) {
            int max_r = LIMIT - p - q;
            if (max_r <= 0) continue;

            for (int r : smaller[q]) {
                if (r >= q) continue;
                if (r > max_r) continue;
                if (sp_set.count(r)) {
                    valid_sums.insert(p + q + r);
                }
            }
        }
    }

    long long ans = 0;
    for (int s : valid_sums) ans += s;
    cout << ans << endl;

    return 0;
}
