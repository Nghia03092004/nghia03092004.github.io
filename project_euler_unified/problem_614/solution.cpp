#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int n = 100;
    vector<ll> dp(n + 1, 0);
    dp[0] = 1;
    for (int k = 1; k <= n; k++)
        for (int j = k; j <= n; j++)
            dp[j] += dp[j - k];
    cout << "p(" << n << ") = " << dp[n] << endl;
    return 0;
}
