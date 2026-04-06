/*
 * Problem 952: Integer Points on Ellipses
 *
 * Count integer (x, y) with 3x^2 + 5y^2 <= N, N = 10^6.
 *
 * For each x in [-sqrt(N/3), sqrt(N/3)], count y in [-M, M] where
 * M = floor(sqrt((N - 3x^2) / 5)).
 *
 * Complexity: O(sqrt(N)) time, O(1) space.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long N = 1000000;
    long long count = 0;

    int max_x = (int)sqrt((double)N / 3.0);
    // Adjust for floating-point imprecision
    while (3LL * (max_x + 1) * (max_x + 1) <= N) max_x++;
    while (3LL * max_x * max_x > N) max_x--;

    for (int x = -max_x; x <= max_x; x++) {
        long long rem = N - 3LL * x * x;
        if (rem < 0) continue;

        int max_y = (int)sqrt((double)rem / 5.0);
        // Correct floating-point floor
        while (5LL * (max_y + 1) * (max_y + 1) <= rem) max_y++;
        while (5LL * max_y * max_y > rem) max_y--;

        count += 2 * max_y + 1;
    }

    cout << count << endl;
    return 0;
}
