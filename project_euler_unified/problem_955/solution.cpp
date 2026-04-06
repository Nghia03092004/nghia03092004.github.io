/*
 * Problem 955: Fibonacci Entry Points
 *
 * For each m = 1..10^5, find alpha(m) = smallest k >= 1 with m | F_k.
 * Return sum of all alpha(m).
 *
 * Direct search: iterate F_k mod m until F_k = 0.
 * Guaranteed to terminate: Pisano period pi(m) <= 6m.
 *
 * Complexity: O(N * avg_alpha) ~ O(N^2).
 */

#include <bits/stdc++.h>
using namespace std;

int fibonacci_entry_point(int m) {
    if (m == 1) return 1;
    int a = 0, b = 1;
    for (int k = 1; k <= 6 * m + 1; k++) {
        int c = (a + b) % m;
        a = b;
        b = c;
        if (a == 0) return k;
    }
    return -1;  // should not reach
}

int main() {
    const int N = 100000;
    long long total = 0;

    for (int m = 1; m <= N; m++) {
        total += fibonacci_entry_point(m);
    }

    cout << total << endl;
    return 0;
}
