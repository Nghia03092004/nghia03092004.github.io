// IOI 2009 - Archery
// Binary search on starting position; simulate tournament rounds.
// O(N^2 log N) for full binary search, O(N^3) brute-force below.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, R;
    cin >> N >> R;

    // Our rank among all 2N+1 archers (1-indexed; lower = stronger)
    int myRank;
    cin >> myRank;

    vector<int> s(2 * N);
    for (int i = 0; i < 2 * N; i++) {
        cin >> s[i];
    }

    // After at most 2N rounds the system is periodic.
    int R_eff = min(R, 2 * N);
    int me = 2 * N; // our internal id

    // Simulate tournament with us starting at target 'startPos' (0-indexed).
    // Returns the target we end up at after R_eff rounds.
    auto simulate = [&](int startPos) -> int {
        vector<vector<int>> targets(N);

        // Place archers: 2 per target, except at startPos we replace one slot.
        int idx = 0;
        for (int t = 0; t < N; t++) {
            if (t == startPos) {
                targets[t].push_back(me);
                targets[t].push_back(idx++);
            } else {
                targets[t].push_back(idx++);
                targets[t].push_back(idx++);
            }
        }

        // Returns true if archer x beats archer y.
        auto stronger = [&](int x, int y) -> bool {
            int sx = (x == me) ? myRank : (x + 1);
            int sy = (y == me) ? myRank : (y + 1);
            return sx < sy;
        };

        int myTarget = startPos;
        for (int r = 0; r < R_eff; r++) {
            vector<vector<int>> newTargets(N);
            for (int t = 0; t < N; t++) {
                int a = targets[t][0], b = targets[t][1];
                int winner, loser;
                if (stronger(a, b)) { winner = a; loser = b; }
                else                { winner = b; loser = a; }

                // Winner moves towards target 0, loser towards target N-1.
                int wDest = max(0, t - 1);
                int lDest = min(N - 1, t + 1);
                newTargets[wDest].push_back(winner);
                newTargets[lDest].push_back(loser);
            }
            targets = newTargets;

            // Locate ourselves.
            for (int t = 0; t < N; t++) {
                for (int x : targets[t]) {
                    if (x == me) myTarget = t;
                }
            }
        }
        return myTarget;
    };

    // Try every starting position; pick the one giving the best (lowest) target.
    int bestTarget = N, bestStart = 0;
    for (int p = 0; p < N; p++) {
        int result = simulate(p);
        if (result < bestTarget) {
            bestTarget = result;
            bestStart = p;
        }
    }

    cout << bestStart + 1 << "\n"; // 1-indexed output
    return 0;
}
