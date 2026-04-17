#include <bits/stdc++.h>
using namespace std;

int main() {
    const int target = 200;
    const int coins[] = {1, 2, 5, 10, 20, 50, 100, 200};
    int dp[target + 1] = {};
    dp[0] = 1;
    for (int c : coins)
        for (int j = c; j <= target; j++)
            dp[j] += dp[j - c];
    cout << dp[target] << endl;  // 73682
    return 0;
}
