#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 100;
    vector<long long> dp(N + 1, 0);
    dp[0] = 1;

    // Partition DP: for each part size k, update dp[j] += dp[j - k]
    for (int k = 1; k <= N; k++)
        for (int j = k; j <= N; j++)
            dp[j] += dp[j - k];

    // p(100) - 1: exclude the trivial partition 100 = 100
    cout << dp[N] - 1 << endl;
    return 0;
}
