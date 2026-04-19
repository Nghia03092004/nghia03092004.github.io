// IOI 2010 - Quality of Living
// Binary search on the median value + 2D prefix sums.
// O(R * C * log(R * C)) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, H, W;
    cin >> R >> C >> H >> W;

    vector<vector<int>> grid(R, vector<int>(C));
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            cin >> grid[i][j];

    int total = H * W;
    int medianPos = (total + 1) / 2; // 1-indexed rank of the median

    // Check: is there an HxW subgrid with at least medianPos values <= m?
    auto check = [&](int m) -> bool {
        vector<vector<int>> psum(R + 1, vector<int>(C + 1, 0));
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                psum[i + 1][j + 1] = (grid[i][j] <= m ? 1 : 0)
                    + psum[i][j + 1] + psum[i + 1][j] - psum[i][j];
            }
        }
        for (int i = H; i <= R; i++) {
            for (int j = W; j <= C; j++) {
                int cnt = psum[i][j] - psum[i - H][j]
                        - psum[i][j - W] + psum[i - H][j - W];
                if (cnt >= medianPos) return true;
            }
        }
        return false;
    };

    int lo = 1, hi = R * C, ans = hi;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (check(mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    cout << ans << "\n";
    return 0;
}
