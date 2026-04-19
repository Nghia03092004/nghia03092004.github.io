// IOI 1991 - Problem 3: Matrix Game
// Two-player game on m x n matrix. Players alternate removing a row or column,
// scoring its sum. Player 1 maximizes, Player 2 minimizes Player 1's total.
// Minimax with bitmask memoization. Feasible for m,n <= 10.
#include <bits/stdc++.h>
using namespace std;

int m, n;
int A[12][12];
unordered_map<long long, int> memo;

int rowSum(int r, int colMask) {
    int s = 0;
    for (int c = 0; c < n; c++)
        if (colMask & (1 << c)) s += A[r][c];
    return s;
}

int colSum(int c, int rowMask) {
    int s = 0;
    for (int r = 0; r < m; r++)
        if (rowMask & (1 << r)) s += A[r][c];
    return s;
}

// Returns Player 1's optimal score from this state onward
int solve(int rowMask, int colMask, bool isMax) {
    if (rowMask == 0 || colMask == 0) return 0;

    long long key = ((long long)rowMask << 20) | ((long long)colMask << 8) | isMax;
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int best = isMax ? INT_MIN : INT_MAX;

    // Try removing each remaining row
    for (int r = 0; r < m; r++) {
        if (!(rowMask & (1 << r))) continue;
        int s = rowSum(r, colMask);
        int future = solve(rowMask ^ (1 << r), colMask, !isMax);
        if (isMax)
            best = max(best, s + future);
        else
            best = min(best, future); // Player 2 takes s, not added to P1
    }

    // Try removing each remaining column
    for (int c = 0; c < n; c++) {
        if (!(colMask & (1 << c))) continue;
        int s = colSum(c, rowMask);
        int future = solve(rowMask, colMask ^ (1 << c), !isMax);
        if (isMax)
            best = max(best, s + future);
        else
            best = min(best, future);
    }

    return memo[key] = best;
}

int main() {
    scanf("%d%d", &m, &n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("%d\n", solve((1 << m) - 1, (1 << n) - 1, true));
    return 0;
}
