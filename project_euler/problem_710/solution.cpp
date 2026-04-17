#include <bits/stdc++.h>
using namespace std;
int main() {
    int N = 1000;
    vector<long long> dp(N + 1, 0);
    dp[0] = 1;
    for (int k = 1; k <= N; k++)
        for (int j = N; j >= 2*k; j--)
            dp[j] += dp[j - 2*k];

    for (int n = 1; n <= N; n++) {
        long long t = dp[n];
        for (int c = 1; c <= n; c++)
            t += dp[n - c];
        if (t > 1000000) {
            cout << n << endl;
            return 0;
        }
    }
    return 0;
}
