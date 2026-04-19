// IOI 2004 - Phidias
// Minimize waste when cutting a W x H slab into desired piece sizes.
// DP on dimensions; cuts only at desired piece widths/heights.
// O(W * H * N) where N = number of piece types.
#include <bits/stdc++.h>
using namespace std;

int dp[601][601];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int W, H;
    cin >> W >> H;

    int N;
    cin >> N;

    vector<int> pw(N), ph(N);
    set<int> ws, hs;
    set<pair<int, int>> pieces;
    for (int i = 0; i < N; i++) {
        cin >> pw[i] >> ph[i];
        ws.insert(pw[i]);
        hs.insert(ph[i]);
        pieces.insert({pw[i], ph[i]});
    }

    for (int a = 1; a <= W; a++) {
        for (int b = 1; b <= H; b++) {
            if (pieces.count({a, b})) {
                dp[a][b] = 0;
                continue;
            }
            dp[a][b] = a * b; // worst case: all waste

            // Vertical cuts at desired piece widths
            for (int w : ws) {
                if (w >= a) break;
                dp[a][b] = min(dp[a][b], dp[w][b] + dp[a - w][b]);
            }
            // Horizontal cuts at desired piece heights
            for (int h : hs) {
                if (h >= b) break;
                dp[a][b] = min(dp[a][b], dp[a][h] + dp[a][b - h]);
            }
        }
    }

    cout << dp[W][H] << "\n";
    return 0;
}
