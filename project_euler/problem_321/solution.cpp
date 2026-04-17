#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 321: Swapping Counters
 *
 * n(n+2) = k(k+1)/2  <==>  x^2 - 8y^2 = -7  (x = 2k+1, y = n+1).
 * Two families of solutions with recurrence n_{i+2} = 6 n_{i+1} - n_i + 4.
 * Family A seeds: n = 0, 3.   Family B seeds: n = 1, 10.
 */

int main() {
    vector<long long> vals;

    // Family A (skip n = 0)
    long long a0 = 0, a1 = 3;
    vals.push_back(a1);
    for (int i = 2; i < 25; i++) {
        long long a2 = 6 * a1 - a0 + 4;
        vals.push_back(a2);
        a0 = a1;
        a1 = a2;
    }

    // Family B
    long long b0 = 1, b1 = 10;
    vals.push_back(b0);
    vals.push_back(b1);
    for (int i = 2; i < 25; i++) {
        long long b2 = 6 * b1 - b0 + 4;
        vals.push_back(b2);
        b0 = b1;
        b1 = b2;
    }

    sort(vals.begin(), vals.end());

    long long ans = 0;
    for (int i = 0; i < 40; i++)
        ans += vals[i];

    cout << ans << endl;
    return 0;
}
