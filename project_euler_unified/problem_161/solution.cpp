#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 161: Triominoes
 *
 * Count the number of ways to tile a 9x12 grid with triominoes
 * (L-shaped and I-shaped) via broken-profile dynamic programming.
 */

static const int ROWS = 9;
static const int COLS = 12;

// 6 triomino placements: offsets (dr, dc) from anchor cell
static const int TRI[6][3][2] = {
    {{0, 0}, {0, 1}, {1, 0}},   // L-A
    {{0, 0}, {0, 1}, {1, 1}},   // L-B
    {{0, 0}, {1, 0}, {1, 1}},   // L-C
    {{0, 0}, {1, -1}, {1, 0}},  // L-D
    {{0, 0}, {0, 1}, {0, 2}},   // I-horizontal
    {{0, 0}, {1, 0}, {2, 0}},   // I-vertical
};

int main() {
    unordered_map<int, long long> dp, ndp;
    dp[0] = 1;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            ndp.clear();
            for (auto &[mask, ways] : dp) {
                if (mask & 1) {
                    ndp[mask >> 1] += ways;
                } else {
                    for (int t = 0; t < 6; t++) {
                        bool ok = true;
                        int nmask = mask;
                        for (int k = 0; k < 3; k++) {
                            int nr = r + TRI[t][k][0];
                            int nc = c + TRI[t][k][1];
                            if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) {
                                ok = false;
                                break;
                            }
                            int bit = TRI[t][k][0] * COLS + TRI[t][k][1];
                            if (bit < 0 || (nmask & (1 << bit))) {
                                ok = false;
                                break;
                            }
                            nmask |= 1 << bit;
                        }
                        if (ok)
                            ndp[nmask >> 1] += ways;
                    }
                }
            }
            swap(dp, ndp);
        }
    }

    cout << dp[0] << endl;
    return 0;
}
