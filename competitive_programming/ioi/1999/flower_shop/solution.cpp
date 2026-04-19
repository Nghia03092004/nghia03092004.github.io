// IOI 1999 - Flower Shop
// Place F flowers into V vases (F <= V) in strictly increasing vase order
// to maximize total preference. Output max preference and assignment.
// Approach: DP with running max optimization, O(F*V) time, O(F*V) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int F, V;
    cin >> F >> V;

    vector<vector<int>> pref(F + 1, vector<int>(V + 1));
    for (int i = 1; i <= F; i++)
        for (int j = 1; j <= V; j++)
            cin >> pref[i][j];

    const int NEG_INF = -1e9;

    // dp[i][j] = best total placing flowers 1..i with flower i in vase j
    vector<vector<int>> dp(F + 1, vector<int>(V + 1, NEG_INF));
    vector<vector<int>> par(F + 1, vector<int>(V + 1, 0));

    // Base case: flower 1 can go in vases 1..V-F+1
    for (int j = 1; j <= V - F + 1; j++) {
        dp[1][j] = pref[1][j];
    }

    // Fill DP with running max over dp[i-1][k] for k < j
    for (int i = 2; i <= F; i++) {
        int best = NEG_INF;
        int bestk = 0;
        for (int j = i; j <= V - F + i; j++) {
            // Update running max from dp[i-1][j-1]
            if (dp[i - 1][j - 1] > best) {
                best = dp[i - 1][j - 1];
                bestk = j - 1;
            }
            if (best > NEG_INF) {
                dp[i][j] = best + pref[i][j];
                par[i][j] = bestk;
            }
        }
    }

    // Find best answer among all valid ending vases
    int ans = NEG_INF, lastVase = 0;
    for (int j = F; j <= V; j++) {
        if (dp[F][j] > ans) {
            ans = dp[F][j];
            lastVase = j;
        }
    }

    if (ans <= NEG_INF) {
        cout << -1 << "\n";
        return 0;
    }

    cout << ans << "\n";

    // Reconstruct assignment
    vector<int> assignment(F + 1);
    assignment[F] = lastVase;
    for (int i = F; i >= 2; i--) {
        assignment[i - 1] = par[i][assignment[i]];
    }

    for (int i = 1; i <= F; i++) {
        cout << assignment[i] << (i < F ? ' ' : '\n');
    }

    return 0;
}
