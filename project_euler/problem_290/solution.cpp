#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count 0 <= n < 10^18 with S(n) = S(137*n)
    // Digit DP: process digits from least significant to most significant
    // State: (position, carry, diff) where
    //   carry = carry from 137*n multiplication
    //   diff = S(n) - S(137*n) accumulated so far

    const int DIGITS = 18;
    const int MAX_CARRY = 137; // max carry: floor((137*9 + 136)/10) = 136, so 0..136
    const int DIFF_OFFSET = 162; // diff ranges from -162 to 162
    const int DIFF_RANGE = 325;

    // dp[carry][diff+offset] = count of numbers
    // Process digit by digit from position 0 to DIGITS-1

    vector<vector<long long>> dp(MAX_CARRY, vector<long long>(DIFF_RANGE, 0));
    dp[0][DIFF_OFFSET] = 1; // carry=0, diff=0

    for (int pos = 0; pos < DIGITS; pos++) {
        vector<vector<long long>> ndp(MAX_CARRY, vector<long long>(DIFF_RANGE, 0));
        for (int carry = 0; carry < MAX_CARRY; carry++) {
            for (int di = 0; di < DIFF_RANGE; di++) {
                if (dp[carry][di] == 0) continue;
                long long cnt = dp[carry][di];
                for (int y = 0; y <= 9; y++) {
                    int val = 137 * y + carry;
                    int new_carry = val / 10;
                    int v = val % 10;
                    int new_di = di + (y - v);
                    if (new_di >= 0 && new_di < DIFF_RANGE && new_carry < MAX_CARRY) {
                        ndp[new_carry][new_di] += cnt;
                    }
                }
            }
        }
        dp = ndp;
    }

    // After processing all digits of n, the remaining carry c represents
    // additional upper digits of 137*n. Their digit sum S(c) must equal
    // the accumulated diff: diff = S(carry).
    auto digitSum = [](int x) {
        int s = 0;
        while (x > 0) { s += x % 10; x /= 10; }
        return s;
    };

    long long ans = 0;
    for (int carry = 0; carry < MAX_CARRY; carry++) {
        int ds = digitSum(carry);
        int di = DIFF_OFFSET + ds;
        if (di >= 0 && di < DIFF_RANGE) {
            ans += dp[carry][di];
        }
    }
    cout << ans << endl;

    return 0;
}
