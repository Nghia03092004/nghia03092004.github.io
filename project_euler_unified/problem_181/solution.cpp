#include <bits/stdc++.h>
using namespace std;

int main() {
    const int B = 60, W = 40;
    vector<vector<long long>> dp(B + 1, vector<long long>(W + 1, 0));
    dp[0][0] = 1;

    for (int db = 0; db <= B; db++) {
        for (int dw = 0; dw <= W; dw++) {
            if (db == 0 && dw == 0) continue;
            for (int b = db; b <= B; b++) {
                for (int w = dw; w <= W; w++) {
                    dp[b][w] += dp[b - db][w - dw];
                }
            }
        }
    }

    cout << dp[B][W] << endl;
    return 0;
}
