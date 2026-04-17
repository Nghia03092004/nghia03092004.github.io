#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 229: Four Representations Using Squares
    // Count n <= 2*10^9 representable as a^2+b^2, a^2+7c^2, a^2+11d^2, a^2+13e^2
    // simultaneously.
    //
    // Approach: segmented sieve with bit arrays.
    // For each segment [lo, hi), mark representable numbers for all 4 forms,
    // AND the results, and count.

    const long long N = 2000000000LL;
    const int BLOCK = 10000000; // 10^7 per block
    const int K[] = {1, 7, 11, 13};
    const int NK = 4;

    int count = 0;

    for (long long lo = 0; lo <= N; lo += BLOCK) {
        long long hi = min(lo + BLOCK, N + 1);
        int sz = (int)(hi - lo);

        // result[i] = 1 if lo+i is representable in all 4 forms
        vector<bool> result(sz, true);

        for (int ki = 0; ki < NK; ki++) {
            int k = K[ki];
            vector<bool> repr(sz, false);

            // b from 0 to sqrt(hi / k)
            long long b_max = (long long)sqrt((double)hi / k) + 1;
            for (long long b = 0; b * b * k < hi; b++) {
                long long kb2 = k * b * b;
                if (kb2 > N) break;
                // a from 0 to sqrt(hi - kb2)
                long long a_start_sq = (lo > kb2) ? lo - kb2 : 0;
                long long a_start = (long long)sqrt((double)a_start_sq);
                if (a_start > 0) a_start--;
                while (a_start * a_start + kb2 < lo && a_start * a_start + kb2 <= N)
                    a_start++;

                for (long long a = a_start; ; a++) {
                    long long val = a * a + kb2;
                    if (val >= hi) break;
                    if (val > N) break;
                    if (val >= lo) {
                        repr[(int)(val - lo)] = true;
                    }
                }
            }

            // AND with result
            for (int i = 0; i < sz; i++)
                if (!repr[i]) result[i] = false;
        }

        for (int i = 0; i < sz; i++)
            if (result[i] && lo + i >= 1) // n >= 1
                count++;
    }

    printf("%d\n", count);
    return 0;
}
