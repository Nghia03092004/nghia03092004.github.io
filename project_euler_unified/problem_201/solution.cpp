#include <bits/stdc++.h>
using namespace std;

int main(){
    // S = {1^2, 2^2, ..., 100^2}, find sum of all v such that exactly one
    // 50-element subset of S sums to v.

    const int N = 100, K = 50;
    // Compute elements
    vector<int> a(N);
    for(int i = 0; i < N; i++) a[i] = (i+1)*(i+1);

    int total = 0;
    for(int x : a) total += x; // 338350
    int minsum = 0;
    for(int i = 0; i < K; i++) minsum += a[i]; // 42925
    int maxsum = total - minsum; // 295425

    // dp[j][s] in {0,1,2} meaning 0/1/2+ subsets of size j summing to s
    // Use bytes to save memory. We iterate elements and update.
    // dp dimensions: (K+1) x (maxsum+1)

    vector<vector<uint8_t>> dp(K+1, vector<uint8_t>(maxsum+1, 0));
    dp[0][0] = 1;

    for(int i = 0; i < N; i++){
        int ai = a[i];
        // Iterate j in reverse to avoid reuse
        for(int j = min(i+1, K); j >= 1; j--){
            for(int s = maxsum; s >= ai; s--){
                if(dp[j-1][s-ai]){
                    int val = dp[j][s] + dp[j-1][s-ai];
                    if(val > 2) val = 2;
                    dp[j][s] = val;
                }
            }
        }
    }

    long long ans = 0;
    for(int s = 0; s <= maxsum; s++){
        if(dp[K][s] == 1) ans += s;
    }

    cout << ans << endl;
    return 0;
}
