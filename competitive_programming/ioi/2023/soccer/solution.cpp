#include <bits/stdc++.h>
using namespace std;

// IOI 2023 - Soccer Stadium
// N x N grid with trees. Find largest "regular stadium": a set of empty cells
// where any two cells connect via at most 2 straight kicks (axis-aligned
// segments fully within the set).
//
// Approaches combined:
//   1. Single relay column: for each column c, sum maximal row intervals through c.
//   2. Single relay row: symmetric to approach 1.
//   3. Maximal empty rectangle (histogram method).
//   4. Relay column interval [c1,c2]: for each (c1,c2) pair, find rows where
//      [c1,c2] is fully empty, extend each row's interval left/right.
//      This is the key O(N^3) approach that covers all optimal stadiums.

int biggest_stadium(int N, vector<vector<int>> F) {
    int best = 0;

    // Precompute for each cell: leftmost and rightmost empty extent in its row
    vector<vector<int>> left_ext(N, vector<int>(N));
    vector<vector<int>> right_ext(N, vector<int>(N));
    for (int r = 0; r < N; r++) {
        // Left extent: furthest left we can go from (r, c) within empty cells
        left_ext[r][0] = (F[r][0] == 0) ? 0 : -1;
        for (int c = 1; c < N; c++) {
            if (F[r][c] == 1)
                left_ext[r][c] = -1;
            else
                left_ext[r][c] = (left_ext[r][c - 1] >= 0) ? left_ext[r][c - 1] : c;
        }
        // Right extent
        right_ext[r][N - 1] = (F[r][N - 1] == 0) ? N - 1 : -1;
        for (int c = N - 2; c >= 0; c--) {
            if (F[r][c] == 1)
                right_ext[r][c] = -1;
            else
                right_ext[r][c] = (right_ext[r][c + 1] >= 0) ? right_ext[r][c + 1] : c;
        }
    }

    // Approach 1: single relay column
    for (int c = 0; c < N; c++) {
        int total = 0;
        for (int r = 0; r < N; r++) {
            if (F[r][c] == 0)
                total += right_ext[r][c] - left_ext[r][c] + 1;
        }
        best = max(best, total);
    }

    // Approach 2: single relay row
    // Precompute vertical extents
    vector<vector<int>> top_ext(N, vector<int>(N));
    vector<vector<int>> bot_ext(N, vector<int>(N));
    for (int c = 0; c < N; c++) {
        top_ext[0][c] = (F[0][c] == 0) ? 0 : -1;
        for (int r = 1; r < N; r++) {
            if (F[r][c] == 1)
                top_ext[r][c] = -1;
            else
                top_ext[r][c] = (top_ext[r - 1][c] >= 0) ? top_ext[r - 1][c] : r;
        }
        bot_ext[N - 1][c] = (F[N - 1][c] == 0) ? N - 1 : -1;
        for (int r = N - 2; r >= 0; r--) {
            if (F[r][c] == 1)
                bot_ext[r][c] = -1;
            else
                bot_ext[r][c] = (bot_ext[r + 1][c] >= 0) ? bot_ext[r + 1][c] : r;
        }
    }
    for (int r = 0; r < N; r++) {
        int total = 0;
        for (int c = 0; c < N; c++) {
            if (F[r][c] == 0)
                total += bot_ext[r][c] - top_ext[r][c] + 1;
        }
        best = max(best, total);
    }

    // Approach 3: maximal empty rectangle (histogram)
    vector<int> height(N, 0);
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++)
            height[c] = (F[r][c] == 0) ? height[c] + 1 : 0;
        stack<int> stk;
        for (int c = 0; c <= N; c++) {
            int h = (c < N) ? height[c] : 0;
            while (!stk.empty() && height[stk.top()] > h) {
                int idx = stk.top(); stk.pop();
                int width = stk.empty() ? c : c - stk.top() - 1;
                best = max(best, height[idx] * width);
            }
            stk.push(c);
        }
    }

    // Approach 4: relay column interval [c1, c2]
    // Precompute prefix sums for each row to check if [c1,c2] is tree-free
    vector<vector<int>> row_prefix(N, vector<int>(N + 1, 0));
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            row_prefix[r][c + 1] = row_prefix[r][c] + F[r][c];

    for (int c1 = 0; c1 < N; c1++) {
        for (int c2 = c1; c2 < N; c2++) {
            int total = 0;
            for (int r = 0; r < N; r++) {
                // Check if [c1, c2] is entirely empty in row r
                if (row_prefix[r][c2 + 1] - row_prefix[r][c1] == 0) {
                    // Extend left and right from [c1, c2]
                    int left = left_ext[r][c1];
                    int right = right_ext[r][c2];
                    total += right - left + 1;
                }
            }
            best = max(best, total);
        }
    }

    return best;
}

int main() {
    int N;
    scanf("%d", &N);
    vector<vector<int>> F(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &F[i][j]);
    printf("%d\n", biggest_stadium(N, F));
    return 0;
}
