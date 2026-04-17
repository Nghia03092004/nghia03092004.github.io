#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 662: Fibonacci Paths
 *
 * Count paths from (0,0) to (W,H) where each step has Fibonacci length.
 * F(W,H) mod 10^9+7.
 *
 * Step (dx,dy): dx,dy >= 0, dx^2+dy^2 = fib_k^2 for some Fibonacci fib_k.
 * DP: f[x][y] = sum over valid (dx,dy) of f[x-dx][y-dy].
 */

const int MOD = 1e9 + 7;

int main() {
    // Generate Fibonacci numbers
    vector<long long> fibs = {1, 2};
    while (fibs.back() < 20000) {
        fibs.push_back(fibs[fibs.size()-1] + fibs[fibs.size()-2]);
    }

    int W = 10, H = 10; // Small test case

    // Generate valid steps
    vector<pair<int,int>> steps;
    for (long long f : fibs) {
        if (f > max(W, H)) break;
        long long f2 = f * f;
        for (long long x = 0; x <= f; x++) {
            long long y2 = f2 - x * x;
            long long y = (long long)round(sqrt((double)y2));
            if (y * y == y2 && y >= 0 && x <= W && y <= H) {
                if (x > 0 || y > 0)
                    steps.push_back({(int)x, (int)y});
            }
        }
    }

    // Remove duplicates
    sort(steps.begin(), steps.end());
    steps.erase(unique(steps.begin(), steps.end()), steps.end());

    printf("Number of valid steps: %d\n", (int)steps.size());

    // DP
    vector<vector<long long>> dp(W + 1, vector<long long>(H + 1, 0));
    dp[0][0] = 1;

    for (int x = 0; x <= W; x++) {
        for (int y = 0; y <= H; y++) {
            if (dp[x][y] == 0) continue;
            for (auto [dx, dy] : steps) {
                int nx = x + dx, ny = y + dy;
                if (nx <= W && ny <= H) {
                    dp[nx][ny] = (dp[nx][ny] + dp[x][y]) % MOD;
                }
            }
        }
    }

    printf("F(%d,%d) = %lld\n", W, H, dp[W][H]);

    // Verify
    assert(dp[W][H] == 215846462);
    printf("Verification passed!\n");

    return 0;
}
