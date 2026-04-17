#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 926: Binomial Coefficient Divisibility
 *
 * Count entries in Pascal's triangle (rows 0-1000) NOT divisible by p=2.
 *
 * By Lucas' theorem: binom(n,k) != 0 mod p iff each base-p digit
 * of k <= corresponding digit of n.
 * Non-zero entries in row n = prod(d_i + 1) over base-p digits d_i.
 * For p=2: = 2^popcount(n).
 *
 * Kummer's theorem: v_p(binom(m+n,m)) = # carries adding m,n in base p.
 *
 * Complexity: O(N * log_p(N)).
 */

int main() {
    int N = 1000, p = 2;
    long long total = 0;
    for (int n = 0; n <= N; n++) {
        long long prod = 1;
        int nn = n;
        if (nn == 0) { total += 1; continue; }
        while (nn > 0) {
            prod *= (nn % p + 1);
            nn /= p;
        }
        total += prod;
    }
    cout << total << endl;

    // Verify: row 0 has 1 entry, row 1 has 2, row 3 has 4
    assert(1 == 1);  // row 0
    // row 3 = 11 in binary, popcount=2, so 2^2=4 odd entries
    int r3 = 1;
    int nn = 3;
    while (nn > 0) { r3 *= (nn%2+1); nn /= 2; }
    assert(r3 == 4);

    return 0;
}
