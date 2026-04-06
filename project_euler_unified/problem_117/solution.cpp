#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 117: Red, Green, and Blue Tiles
 * Tetranacci recurrence: f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4)
 * f(0) = 1, f(1) = 1, f(2) = 2, f(3) = 4.
 * Answer: f(50) = 100808458960497.
 */

int main() {
    const int N = 50;
    vector<long long> f(N + 1, 0);
    f[0] = 1;
    for (int i = 1; i <= N; i++) {
        for (int L = 1; L <= 4 && L <= i; L++) {
            f[i] += f[i - L];
        }
    }

    // Cross-checks
    assert(f[0] == 1);
    assert(f[1] == 1);
    assert(f[2] == 2);
    assert(f[3] == 4);
    assert(f[4] == 8);
    assert(f[N] == 100808458960497LL);

    cout << f[N] << endl;
    return 0;
}
