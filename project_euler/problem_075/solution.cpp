#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count singular perimeters L <= 1,500,000.
    // Generate primitive Pythagorean triples via Euclid: L0 = 2m(m+n).
    // Mark all multiples kL0. Count perimeters hit exactly once.

    const int LMAX = 1500000;
    vector<int> cnt(LMAX + 1, 0);

    int mlimit = (int)sqrt((double)LMAX / 2.0) + 1;

    for (int m = 2; m <= mlimit; m++) {
        for (int n = 1; n < m; n++) {
            if ((m - n) % 2 == 0) continue;
            if (__gcd(m, n) != 1) continue;

            int L0 = 2 * m * (m + n);
            if (L0 > LMAX) break;

            for (int L = L0; L <= LMAX; L += L0) {
                cnt[L]++;
            }
        }
    }

    int answer = 0;
    for (int L = 1; L <= LMAX; L++) {
        if (cnt[L] == 1) answer++;
    }

    cout << answer << endl;
    return 0;
}
