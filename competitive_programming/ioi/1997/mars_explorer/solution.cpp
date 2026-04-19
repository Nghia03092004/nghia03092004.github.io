// IOI 1997 - Mars Explorer
// 3 simultaneous paths from (1,1) to (n,n), maximize distinct cells visited
// DP on (step, r1, r2, r3) with r1 <= r2 <= r3 symmetry optimization
// Time: O(n^4), Space: O(n^4)
#include <bits/stdc++.h>
using namespace std;

int n;
int grid[32][32]; // 1 = free, 0 = obstacle
int dp[62][32][32][32];

// Count distinct valid cells at step t for rows r1 <= r2 <= r3
// Returns -1 if any cell is invalid
int countDistinct(int t, int r1, int r2, int r3) {
    int c1 = t - r1 + 2, c2 = t - r2 + 2, c3 = t - r3 + 2;
    if (r1 < 1 || r1 > n || c1 < 1 || c1 > n || !grid[r1][c1]) return -1;
    if (r2 < 1 || r2 > n || c2 < 1 || c2 > n || !grid[r2][c2]) return -1;
    if (r3 < 1 || r3 > n || c3 < 1 || c3 > n || !grid[r3][c3]) return -1;

    int cnt = 1;
    if (r2 != r1) cnt++;
    if (r3 != r1 && r3 != r2) cnt++;
    return cnt;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            scanf("%d", &grid[i][j]);

    memset(dp, -1, sizeof(dp));

    int maxT = 2 * (n - 1);

    // Base: t=0, all paths at (1,1)
    if (grid[1][1])
        dp[0][1][1][1] = 1;

    for (int t = 0; t < maxT; t++) {
        for (int r1 = 1; r1 <= n; r1++) {
            int c1 = t - r1 + 2;
            if (c1 < 1 || c1 > n) continue;
            for (int r2 = r1; r2 <= n; r2++) {
                int c2 = t - r2 + 2;
                if (c2 < 1 || c2 > n) continue;
                for (int r3 = r2; r3 <= n; r3++) {
                    int c3 = t - r3 + 2;
                    if (c3 < 1 || c3 > n) continue;
                    if (dp[t][r1][r2][r3] < 0) continue;

                    int cur = dp[t][r1][r2][r3];

                    // Each path moves right (r stays) or down (r+1)
                    for (int d1 = 0; d1 <= 1; d1++)
                    for (int d2 = 0; d2 <= 1; d2++)
                    for (int d3 = 0; d3 <= 1; d3++) {
                        int nr[3] = {r1 + d1, r2 + d2, r3 + d3};
                        sort(nr, nr + 3); // enforce ordering

                        int add = countDistinct(t + 1, nr[0], nr[1], nr[2]);
                        if (add < 0) continue;

                        int& ref = dp[t + 1][nr[0]][nr[1]][nr[2]];
                        ref = max(ref, cur + add);
                    }
                }
            }
        }
    }

    printf("%d\n", dp[maxT][n][n][n]);
    return 0;
}
