#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 162: Hexadecimal Numbers
 *
 * Count hex numbers with at most 16 digits containing all of {0, 1, A}.
 * Inclusion-exclusion over missing-digit sets.
 */

typedef unsigned __int128 u128;

int main() {
    auto power = [](u128 base, int exp) -> u128 {
        u128 r = 1;
        for (int i = 0; i < exp; i++) r *= base;
        return r;
    };

    u128 total = 0;
    for (int n = 1; n <= 16; n++) {
        u128 S   = 15 * power(16, n - 1);
        u128 A0  = power(15, n);
        u128 A1  = 14 * power(15, n - 1);
        u128 AA  = 14 * power(15, n - 1);
        u128 A01 = power(14, n);
        u128 A0A = power(14, n);
        u128 A1A = 13 * power(14, n - 1);
        u128 A01A = power(13, n);
        u128 bad = A0 + A1 + AA - A01 - A0A - A1A + A01A;
        total += S - bad;
    }

    // Convert to uppercase hexadecimal
    string hex;
    u128 v = total;
    if (v == 0) hex = "0";
    while (v > 0) {
        int d = (int)(v % 16);
        hex += (d < 10) ? ('0' + d) : ('A' + d - 10);
        v /= 16;
    }
    reverse(hex.begin(), hex.end());
    cout << hex << endl;
    return 0;
}
