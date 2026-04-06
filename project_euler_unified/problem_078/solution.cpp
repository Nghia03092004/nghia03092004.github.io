#include <bits/stdc++.h>
using namespace std;

int main() {
    const int MOD = 1000000;
    const int MAXN = 100000;

    vector<int> p(MAXN + 1, 0);
    p[0] = 1;

    // Precompute generalized pentagonal numbers with signs
    vector<pair<int, int>> pent; // (pentagonal number, sign)
    for (int k = 1; ; k++) {
        int g1 = k * (3 * k - 1) / 2;
        int g2 = k * (3 * k + 1) / 2;
        if (g1 > MAXN) break;
        int sign = (k % 2 == 1) ? 1 : -1;
        pent.push_back({g1, sign});
        if (g2 <= MAXN)
            pent.push_back({g2, sign});
    }

    for (int n = 1; n <= MAXN; n++) {
        long long val = 0;
        for (auto& [g, s] : pent) {
            if (g > n) break;
            val += (long long)s * p[n - g];
        }
        p[n] = ((int)(val % MOD) + MOD) % MOD;
        if (p[n] == 0) {
            cout << n << endl;
            return 0;
        }
    }

    return 0;
}
