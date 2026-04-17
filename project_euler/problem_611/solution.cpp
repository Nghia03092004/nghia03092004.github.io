#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int n = 100;
    vector<ll> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k * k <= i; k++)
            dp[i] += dp[i - k * k];
    }
    cout << dp[n] << endl;
    return 0;
}
