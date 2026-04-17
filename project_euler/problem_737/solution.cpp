#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 737: Coin Loops
 *
 * Stack coins around a line. Compute angular displacement per coin and find N for 2020 loops.
 */


int main() {
    // Compute coins needed for L loops
    int target_loops = 2020;
    double target = 360.0 * target_loops * M_PI / 180.0;
    double total = 0.0;
    int n = 2;
    while (total < target) {
        total += 2.0 * asin(1.0 / (2.0 * sqrt(n - 0.5)));
        n++;
    }
    printf("Coins for %d loops: %d\n", target_loops, n - 1);
    return 0;
}
