// IOI 2000 - Post Office
// Place P post offices among V villages on a line to minimize total distance.
// Each village connects to its nearest post office.
// Approach: DP with precomputed cost w[i][j] for serving villages i..j
// with one optimally-placed (median) post office.
// Complexity: O(V^2 * P) time, O(V^2 + V*P) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, P;
    cin >> V >> P;

    vector<int> x(V + 1);
    for (int i = 1; i <= V; i++) cin >> x[i];

    // Precompute w[i][j] = cost of one post office serving villages i..j
    // Optimal location is the median; w[i][j] = w[i][j-1] + x[j] - x[med]
    vector<vector<int>> w(V + 1, vector<int>(V + 1, 0));
    for (int i = 1; i <= V; i++) {
        for (int j = i + 1; j <= V; j++) {
            int med = (i + j) / 2;
            w[i][j] = w[i][j - 1] + x[j] - x[med];
        }
    }

    // dp[i][j] = min cost for villages 1..i with j post offices
    const int INF = 1e9;
    vector<vector<int>> dp(V + 1, vector<int>(P + 1, INF));
    vector<vector<int>> opt(V + 1, vector<int>(P + 1, 0));

    // Base case: one post office
    for (int i = 1; i <= V; i++) {
        dp[i][1] = w[1][i];
    }

    // Fill DP for 2..P post offices
    for (int j = 2; j <= P; j++) {
        for (int i = V; i >= j; i--) {
            dp[i][j] = INF;
            for (int k = j - 1; k <= i - 1; k++) {
                int cost = dp[k][j - 1] + w[k + 1][i];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    opt[i][j] = k;
                }
            }
        }
    }

    cout << dp[V][P] << "\n";

    // Reconstruct post office positions (placed at medians of each group)
    vector<int> postOffices;
    int curV = V, curP = P;
    while (curP > 1) {
        int k = opt[curV][curP];
        int med = (k + 1 + curV) / 2;
        postOffices.push_back(x[med]);
        curV = k;
        curP--;
    }
    // Last group: villages 1..curV
    int med = (1 + curV) / 2;
    postOffices.push_back(x[med]);

    reverse(postOffices.begin(), postOffices.end());
    for (int i = 0; i < (int)postOffices.size(); i++) {
        cout << postOffices[i] << (i + 1 < (int)postOffices.size() ? ' ' : '\n');
    }

    return 0;
}
