#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 172: 18-digit numbers where each digit appears at most 3 times.
 * Sequential digit DP: for each digit, choose how many positions it fills.
 * Answer = T(18, all caps 3) - T(17, digit-0 cap 2, rest cap 3).
 */

long long compute(int L, vector<int>& maxfreq) {
    vector<vector<long long>> C(L + 1, vector<long long>(L + 1, 0));
    for (int i = 0; i <= L; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }

    vector<long long> dp(L + 1, 0);
    dp[0] = 1;

    for (int dig = 0; dig < 10; dig++) {
        vector<long long> ndp(L + 1, 0);
        for (int j = 0; j <= L; j++) {
            if (dp[j] == 0) continue;
            for (int c = 0; c <= maxfreq[dig] && j + c <= L; c++)
                ndp[j + c] += dp[j] * C[L - j][c];
        }
        dp = ndp;
    }
    return dp[L];
}

int main() {
    vector<int> mf_all(10, 3);
    long long T = compute(18, mf_all);

    vector<int> mf_zero(10, 3);
    mf_zero[0] = 2;
    long long T0 = compute(17, mf_zero);

    cout << T - T0 << endl;
    return 0;
}
