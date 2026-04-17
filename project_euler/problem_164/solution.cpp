#include <bits/stdc++.h>
using namespace std;

// Problem 164: No three consecutive digits sum > 9
// DP with state (last two digits)

int main() {
    const int DIGITS = 20;

    // dp[a][b] = count of valid numbers ending in digits a, b
    long long dp[10][10] = {};

    // First digit: 1-9, second digit: 0 to 9-first
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9 - a; b++)
            dp[a][b] = 1;

    // Process digits 3 through 20
    for (int pos = 3; pos <= DIGITS; pos++) {
        long long ndp[10][10] = {};
        for (int a = 0; a <= 9; a++) {
            for (int b = 0; b <= 9 - a; b++) {
                if (dp[a][b] == 0) continue;
                int maxc = 9 - a - b;
                for (int c = 0; c <= maxc; c++) {
                    ndp[b][c] += dp[a][b];
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    long long ans = 0;
    for (int a = 0; a <= 9; a++)
        for (int b = 0; b <= 9; b++)
            ans += dp[a][b];

    cout << ans << endl;
    return 0;
}
