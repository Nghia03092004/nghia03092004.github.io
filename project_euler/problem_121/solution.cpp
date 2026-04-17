#include <bits/stdc++.h>
using namespace std;

int main() {
    const int n = 15;
    const int half = n / 2; // 7

    // dp[j] = j-th elementary symmetric polynomial of {1,...,i}
    vector<long long> dp(half + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++)
        for (int j = min(i, half); j >= 1; j--)
            dp[j] += (long long)i * dp[j - 1];

    long long N = 0;
    for (int j = 0; j <= half; j++)
        N += dp[j];

    // Compute (n+1)! = 16!
    long long fact = 1;
    for (int i = 1; i <= n + 1; i++)
        fact *= i;

    cout << fact / N << endl;
    return 0;
}
