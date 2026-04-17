#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 108: Diophantine Reciprocals I
 *
 * 1/x + 1/y = 1/n => (x-n)(y-n) = n^2
 * Number of solutions = ceil(d(n^2) / 2) where d is the divisor function.
 * d(n^2) = product of (2*a_i + 1) for each prime factor p_i^a_i of n.
 * We need ceil(d(n^2)/2) > 1000, i.e., d(n^2) >= 2001.
 *
 * Strategy: search over non-increasing exponent sequences assigned to
 * primes 2, 3, 5, 7, 11, 13, ... to minimize n.
 */

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
const int NPRIMES = 15;
long long best_n;

// Recursive search: assign exponents to primes in order
// idx: current prime index, max_exp: maximum exponent allowed (non-increasing)
// current_n: product so far, current_div: d(n^2) so far
void dfs(int idx, int max_exp, long long current_n, long long current_div) {
    if (current_div >= 2001) {
        best_n = min(best_n, current_n);
        return;
    }
    if (idx >= NPRIMES) return;
    if (current_n > best_n) return;

    // Try exponents from max_exp down to 1
    long long pw = 1;
    for (int e = 1; e <= max_exp; e++) {
        pw *= primes[idx];
        if (current_n > best_n / pw) break; // overflow guard
        dfs(idx + 1, e, current_n * pw, current_div * (2 * e + 1));
    }
}

int main() {
    best_n = LLONG_MAX;
    dfs(0, 30, 1, 1);
    cout << best_n << endl;
    return 0;
}
