#include <bits/stdc++.h>
using namespace std;

string solve_puzzle(string s, vector<int> &clues) {
    int n = s.size();
    int k = clues.size();

    // Precompute: can cells [l, r) all be black?
    // prefW[i] = number of forced-white cells in [0, i)
    vector<int> prefW(n + 1, 0), prefB(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefW[i+1] = prefW[i] + (s[i] == '_' ? 1 : 0);
        prefB[i+1] = prefB[i] + (s[i] == 'X' ? 1 : 0);
    }

    auto canBlack = [&](int l, int r) -> bool { // [l, r)
        return prefW[r] - prefW[l] == 0;
    };
    auto canWhite = [&](int i) -> bool {
        return s[i] != 'X';
    };

    // dpL[i][j] = can we place clues 0..j-1 (j clues) in cells 0..i-1?
    // i ranges 0..n, j ranges 0..k
    vector<vector<bool>> dpL(n + 1, vector<bool>(k + 1, false));
    dpL[0][0] = true;

    // Also track: can we reach (i, j) with cell i-1 being white?
    // We need prefix-or for dpL to handle ranges of white cells.

    // Optimized: instead of O(n*k) states with O(n) transitions,
    // use the standard nonogram DP which is O(n*k).

    // dpL[i][j] = true if first i cells can accommodate first j clues
    // Transition 1: cell i is white -> dpL[i+1][j] |= dpL[i][j] (if canWhite(i))
    // Transition 2: place clue j at position [i, i+c[j]-1]:
    //   dpL[i + c[j] + (j < k-1 ? 1 : 0)][j+1] |= dpL[i][j]
    //   if canBlack(i, i+c[j]) and (i+c[j] == n or canWhite(i+c[j]))

    // But this is O(n*k) which might be tight. Let's proceed.

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            if (!dpL[i][j]) continue;
            if (i < n && canWhite(i)) {
                dpL[i+1][j] = true;
            }
            if (j < k) {
                int len = clues[j];
                int end = i + len;
                if (end <= n && canBlack(i, end)) {
                    if (end == n) {
                        dpL[end][j+1] = true;
                    } else if (canWhite(end)) {
                        dpL[end + 1][j + 1] = true;
                    }
                }
            }
        }
    }

    // Check that dpL[n][k] is true before proceeding
    // (problem guarantees a valid solution exists)

    // dpR[i][j] = can we place clues j..k-1 in cells i..n-1?
    vector<vector<bool>> dpR(n + 2, vector<bool>(k + 1, false));
    dpR[n][k] = true;

    for (int i = n - 1; i >= 0; i--) {
        for (int j = k; j >= 0; j--) {
            // Cell i is white: dpR[i][j] |= dpR[i+1][j] if canWhite(i)
            if (canWhite(i) && dpR[i+1][j]) dpR[i][j] = true;
            // Place clue j at position [i, i+c[j]-1]:
            if (j < k) {
                int len = clues[j];
                int end = i + len;
                if (end <= n && canBlack(i, end)) {
                    if (end == n) {
                        if (dpR[end][j+1]) dpR[i][j] = true;
                    } else if (canWhite(end)) {
                        if (dpR[end+1][j+1]) dpR[i][j] = true;
                    }
                }
            }
        }
    }

    // Determine each cell
    string result(n, '?');

    // canBeWhite[i]: exists j such that dpL[i][j] && dpR[i+1][j]
    // Wait: dpL[i][j] means first i cells handle j clues.
    // If cell i is white, then dpL[i+1][j] should be derivable from dpL[i][j].
    // Cell i can be white if exists j: dpL[i][j] && canWhite(i) && dpR[i+1][j]

    vector<bool> possBlack(n, false), possWhite(n, false);

    for (int j = 0; j <= k; j++) {
        for (int i = 0; i < n; i++) {
            // White at position i
            if (canWhite(i) && dpL[i][j] && dpR[i+1][j])
                possWhite[i] = true;

            // Black at position i as part of clue j
            if (j < k) {
                int len = clues[j];
                int start = i;
                int end = start + len;
                if (end <= n && canBlack(start, end) && dpL[start][j]) {
                    bool afterOk;
                    if (end == n) afterOk = dpR[end][j+1];
                    else afterOk = canWhite(end) && dpR[end+1][j+1];

                    if (afterOk) {
                        for (int p = start; p < end; p++)
                            possBlack[p] = true;
                    }
                }
            }
        }
    }

    // The inner loop for marking black is O(n*k*maxLen) which is too slow.
    // Use prefix sums / difference array to mark ranges.

    // Redo black marking with difference array:
    vector<int> blackDiff(n + 1, 0);
    for (int j = 0; j < k; j++) {
        int len = clues[j];
        for (int start = 0; start + len <= n; start++) {
            if (!dpL[start][j]) continue;
            if (!canBlack(start, start + len)) continue;
            int end = start + len;
            bool afterOk;
            if (end == n) afterOk = dpR[end][j+1];
            else afterOk = canWhite(end) && dpR[end+1][j+1];
            if (afterOk) {
                blackDiff[start]++;
                blackDiff[end]--;
            }
        }
    }
    {
        int cur = 0;
        for (int i = 0; i < n; i++) {
            cur += blackDiff[i];
            if (cur > 0) possBlack[i] = true;
        }
    }

    for (int i = 0; i < n; i++) {
        if (possBlack[i] && possWhite[i]) result[i] = '?';
        else if (possBlack[i]) result[i] = 'X';
        else if (possWhite[i]) result[i] = '_';
        else result[i] = '?'; // shouldn't happen
    }

    return result;
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    char buf[n + 1];
    scanf("%s", buf);
    string s(buf);
    vector<int> clues(k);
    for (int i = 0; i < k; i++) scanf("%d", &clues[i]);

    string result = solve_puzzle(s, clues);
    printf("%s\n", result.c_str());
    return 0;
}
