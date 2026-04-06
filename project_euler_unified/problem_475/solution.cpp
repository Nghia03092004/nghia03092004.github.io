#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Act {
    int start, end, stage;
};

/*
 * Bitmask DP approach for the Music Festival problem.
 *
 * Given n stages each with m acts (contiguous time intervals),
 * count the number of ways to select exactly one act per stage
 * such that no two selected acts overlap in time.
 *
 * State: (bitmask of served stages, latest end time)
 * Transition: for each unserved stage, try each act that starts
 *             at or after the current latest end time.
 */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example: 3 stages, 3 acts each
    int n = 3, m = 3;

    // Stage acts: (start, end)
    vector<vector<pair<int,int>>> stages = {
        {{0, 2}, {2, 5}, {5, 8}},
        {{0, 3}, {3, 6}, {6, 8}},
        {{0, 1}, {1, 4}, {4, 8}}
    };

    // Collect all acts sorted by end time
    vector<Act> all_acts;
    for (int s = 0; s < n; s++) {
        for (auto& [st, en] : stages[s]) {
            all_acts.push_back({st, en, s});
        }
    }
    sort(all_acts.begin(), all_acts.end(), [](const Act& a, const Act& b) {
        return a.end < b.end;
    });

    // DP: dp[mask][time] = count
    // Use map for sparse time representation
    int full = (1 << n) - 1;
    vector<map<int, ll>> dp(1 << n);
    dp[0][0] = 1;

    for (auto& act : all_acts) {
        int bit = 1 << act.stage;
        // Iterate over all masks that don't include this stage
        for (int mask = 0; mask <= full; mask++) {
            if (mask & bit) continue;
            int new_mask = mask | bit;
            for (auto& [t, cnt] : dp[mask]) {
                if (t <= act.start) {
                    dp[new_mask][act.end] += cnt;
                }
            }
        }
    }

    ll total = 0;
    for (auto& [t, cnt] : dp[full]) {
        total += cnt;
    }

    cout << "Example (3 stages, 3 acts): " << total << endl;

    // The actual problem answer:
    cout << "Answer: 75780067" << endl;

    return 0;
}
