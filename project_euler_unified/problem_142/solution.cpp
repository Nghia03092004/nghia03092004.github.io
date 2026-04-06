#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 142: Perfect Square Collection
    // Find smallest x+y+z with x>y>z>0, all six sums/differences are perfect squares.
    //
    // Let a^2=x+y, b^2=x-y, c^2=x+z, d^2=x-z, e^2=y+z, f^2=y-z.
    // Relations: a^2 = c^2 + f^2, c^2 = b^2 + e^2, a^2 = d^2 + e^2 (when d<e or d>e)
    // Strategy: for each a, decompose a^2 = c^2 + f^2, then for each e < c,
    //   check if c^2 - e^2 = b^2, and a^2 - e^2 = d^2.

    long long best = LLONG_MAX;

    for (long long a = 3; a <= 1000; a++) {
        long long a2 = a * a;
        for (long long f = 1; f < a; f++) {
            long long c2 = a2 - f * f;
            if (c2 <= 0) break;
            long long c = (long long)sqrt((double)c2);
            if (c * c != c2) continue;
            if (c <= 0) continue;

            for (long long e = 1; e < c; e++) {
                long long b2 = c * c - e * e;
                if (b2 <= 0) continue;
                long long b = (long long)sqrt((double)b2);
                if (b * b != b2) continue;

                long long d2 = a2 - e * e;
                if (d2 <= 0) continue;
                long long d = (long long)sqrt((double)d2);
                if (d * d != d2) continue;

                // Verify b^2 + f^2 = d^2
                if (b2 + f * f != d2) continue;

                // Parity check
                if ((a2 + b2) % 2 != 0) continue;

                long long x = (a2 + b2) / 2;
                long long y = (a2 - b2) / 2;
                long long z = e * e - y;

                if (x > y && y > z && z > 0) {
                    long long s = x + y + z;
                    if (s < best) best = s;
                }
            }
        }
    }

    cout << best << endl;
    return 0;
}
