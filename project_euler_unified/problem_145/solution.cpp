#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 145: Count reversible numbers below 10^9.
    // n is reversible if n + reverse(n) has all odd digits.
    // n must not have trailing zeros.
    //
    // Brute force approach: iterate all n from 1 to 10^9-1.
    // Optimization: n and reverse(n) produce the same sum, so if n < reverse(n),
    // count the pair once (x2). If n == reverse(n) (palindrome), check once.
    // But simpler: just iterate all k-digit numbers.
    //
    // For k=8 we have 9*10^7 numbers to check, which takes ~1-2s in C++.
    // For k=9, answer is 0 (proven analytically), so skip.

    long long total = 0;

    for (int k = 2; k <= 8; k++) {
        // Odd digit counts that are 1 mod 4 yield 0. k=5 is 0 too but let's brute force.
        if (k == 5) continue; // 0 by analysis (and saves time)

        long long lo = 1;
        for (int i = 1; i < k; i++) lo *= 10;
        long long hi = lo * 10;

        long long count = 0;
        for (long long n = lo; n < hi; n++) {
            if (n % 10 == 0) continue;
            long long rev = 0, tmp = n;
            while (tmp > 0) {
                rev = rev * 10 + tmp % 10;
                tmp /= 10;
            }
            long long s = n + rev;
            bool ok = true;
            while (s > 0) {
                if ((s % 10) % 2 == 0) { ok = false; break; }
                s /= 10;
            }
            if (ok) count++;
        }
        total += count;
    }

    // k=9: 0 (no reversible 9-digit numbers)
    cout << total << endl;
    return 0;
}
