#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> devise_strategy(int N) {
    // Strategy table: s[i][j] for whiteboard state i, coin count j
    // s[i][0] = which bag to inspect (0=A, 1=B)
    // s[i][j] for j=1..N: action after seeing j coins
    //   value -1: guess this bag (the inspected one) has fewer
    //   value -2: guess the other bag has fewer
    //   value >= 0: write this value to whiteboard

    // Ternary search approach
    // State encodes (depth, which_bag_last_seen)
    // At each depth, the range is narrowed by factor of 3

    // Compute number of states needed
    int depth = 0;
    {
        int range = N;
        while (range > 1) {
            range = (range + 2) / 3; // ceiling division
            depth++;
        }
    }
    // Total states: 2 * depth (alternating A and B)
    // But we also need a state for each (depth, bag) pair

    int num_states = 0;
    // Each state: (level, inspecting_bag)
    // level 0: full range [1, N], inspect A
    // level 1: narrowed range, inspect B
    // level 2k: inspect A, level 2k+1: inspect B

    // Precompute ranges for each level
    vector<pair<int,int>> ranges; // (lo, hi) at each level
    ranges.push_back({1, N});
    int lo = 1, hi = N;
    while (hi - lo > 0) {
        int len = hi - lo + 1;
        int third = (len + 2) / 3;
        lo = lo + third;
        hi = hi - third;
        if (lo > hi) break;
        ranges.push_back({lo, hi});
    }

    num_states = 2 * ranges.size();
    // Actually, let me reconsider. The number of states is the number of levels.
    // Each level alternates which bag to inspect.

    int max_level = ranges.size();

    vector<vector<int>> strategy(2 * max_level, vector<int>(N + 1, 0));

    for (int level = 0; level < max_level; level++) {
        int state_A = 2 * level;     // inspect A at this level
        int state_B = 2 * level + 1; // inspect B at this level

        int lo_range = ranges[level].first;
        int hi_range = ranges[level].second;
        int len = hi_range - lo_range + 1;
        int third = max(1, (len + 2) / 3);

        int cut1 = lo_range + third - 1;        // end of bottom third
        int cut2 = hi_range - third + 1;         // start of top third
        if (cut2 <= cut1) cut2 = cut1 + 1;

        // State for inspecting A:
        strategy[state_A][0] = 0; // inspect bag A
        for (int v = 1; v <= N; v++) {
            if (v < lo_range) {
                strategy[state_A][v] = -1; // A is definitely smaller
            } else if (v > hi_range) {
                strategy[state_A][v] = -2; // B is definitely smaller
            } else if (v <= cut1) {
                strategy[state_A][v] = -1; // A in bottom third -> A is smaller
            } else if (v >= cut2) {
                strategy[state_A][v] = -2; // A in top third -> B is smaller
            } else {
                // Middle third: transition to next level, inspect B
                if (level + 1 < max_level && 2 * level + 1 < (int)strategy.size())
                    strategy[state_A][v] = state_B;
                else
                    strategy[state_A][v] = -1; // fallback
            }
        }

        // State for inspecting B:
        strategy[state_B][0] = 1; // inspect bag B
        for (int v = 1; v <= N; v++) {
            if (v < lo_range) {
                strategy[state_B][v] = -2; // B is smaller
            } else if (v > hi_range) {
                strategy[state_B][v] = -1; // A is smaller
            } else if (v <= cut1) {
                strategy[state_B][v] = -2; // B in bottom third -> B is smaller
            } else if (v >= cut2) {
                strategy[state_B][v] = -1; // B in top third -> A is smaller
            } else {
                // Middle third: transition to next level, inspect A
                if (level + 1 < max_level)
                    strategy[state_B][v] = 2 * (level + 1);
                else
                    strategy[state_B][v] = -2; // fallback
            }
        }
    }

    return strategy;
}

int main() {
    int N;
    scanf("%d", &N);
    auto strat = devise_strategy(N);
    printf("x = %d\n", (int)strat.size() - 1);
    for (int i = 0; i < (int)strat.size(); i++) {
        printf("State %d (inspect %c):", i, strat[i][0] == 0 ? 'A' : 'B');
        for (int j = 1; j <= min(N, 20); j++)
            printf(" %d", strat[i][j]);
        printf("\n");
    }
    return 0;
}
