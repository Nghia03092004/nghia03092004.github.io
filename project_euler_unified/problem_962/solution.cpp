/*
 * Problem 962: Taxicab Numbers
 *
 * Ta(2) = 1729, Ta(3) = 87539319
 * Answer = (1729 + 87539319) % (10^9 + 7) = 87541048
 *
 * Verification by exhaustive enumeration of a^3 + b^3.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long MOD = 1e9 + 7;

    // Find Ta(2): smallest n = a^3 + b^3 with 2 representations
    map<long long, int> count2;
    long long ta2 = -1;
    for (int b = 1; b <= 13; b++) {
        for (int a = 1; a <= b; a++) {
            long long s = (long long)a*a*a + (long long)b*b*b;
            count2[s]++;
            if (count2[s] >= 2 && (ta2 == -1 || s < ta2)) {
                ta2 = s;
            }
        }
    }

    // Find Ta(3) = 87539319 (verify)
    map<long long, int> count3;
    long long ta3 = -1;
    int lim = 445;
    for (int b = 1; b <= lim; b++) {
        for (int a = 1; a <= b; a++) {
            long long s = (long long)a*a*a + (long long)b*b*b;
            if (s > 100000000) break;
            count3[s]++;
            if (count3[s] >= 3 && (ta3 == -1 || s < ta3)) {
                ta3 = s;
            }
        }
    }

    assert(ta2 == 1729);
    assert(ta3 == 87539319);

    long long ans = (ta2 + ta3) % MOD;
    cout << ans << endl;
    return 0;
}
