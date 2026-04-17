#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 686: Powers of Two
 *
 * Find p(123, 678910): the 678910th smallest j such that 2^j starts with "123".
 *
 * Key insight: 2^j starts with digits L if and only if
 *   log10(L) <= frac(j * log10(2)) < log10(L+1)
 * where frac(x) is the fractional part of x.
 *
 * We iterate j from 0, check the fractional part condition, and count.
 */

int main() {
    const int target = 678910;
    const double log10_2 = log10(2.0);
    const double lo = log10(1.23);  // log10(123) - 2 = log10(1.23)
    const double hi = log10(1.24);  // log10(124) - 2 = log10(1.24)

    int count = 0;
    long long j = 0;

    while (count < target) {
        j++;
        double frac = j * log10_2;
        frac -= floor(frac);
        if (frac >= lo && frac < hi) {
            count++;
        }
    }

    cout << j << endl;
    return 0;
}
