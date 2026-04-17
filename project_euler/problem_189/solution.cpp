#include <bits/stdc++.h>
using namespace std;

// Encode a row's upward triangle colors as a base-3 number
// Row k has k upward triangles, colors 0,1,2

int main() {
    const int N = 8;

    // Row 1: 1 upward triangle, 3 choices
    // State: tuple of colors of upward triangles
    // Represent as vector<int> -> map to count

    // Use map from vector<int> to long long count
    // Row 1
    map<vector<int>, long long> dp;
    for (int c = 0; c < 3; c++) {
        dp[{c}] += 1;
    }

    for (int row = 2; row <= N; row++) {
        // Previous row has (row-1) upward triangles
        // Current row has row upward triangles and (row-1) downward triangles
        // Order in current row: up_1, down_1, up_2, down_2, ..., down_{row-1}, up_row
        // Adjacency within row: consecutive elements differ
        // Adjacency between rows: down_j in current row is adjacent to up_j in previous row

        map<vector<int>, long long> new_dp;

        for (auto& [prev_up, cnt] : dp) {
            // Generate all valid colorings of current row
            // Current row upward triangles: up[0..row-1]
            // Current row downward triangles: down[0..row-2]
            // Constraints:
            //   up[0] != down[0]
            //   down[j] != up[j], down[j] != up[j+1], down[j] != prev_up[j]
            //   (within row: up[j] != down[j-1] for j>=1, and up[j] != down[j] for j<row-1)
            //   These are equivalent to the above since the sequence alternates

            // Build current row incrementally: up[0], down[0], up[1], down[1], ...
            // At each step, choose color satisfying constraints

            // Use recursive generation
            vector<int> cur_up(row), cur_down(row - 1);

            function<void(int)> generate = [&](int pos) {
                // pos goes through the 2*row-1 triangles in order:
                // 0: up[0], 1: down[0], 2: up[1], 3: down[1], ...
                if (pos == 2 * row - 1) {
                    new_dp[cur_up] += cnt;
                    return;
                }

                if (pos % 2 == 0) {
                    // Upward triangle: index pos/2
                    int idx = pos / 2;
                    for (int c = 0; c < 3; c++) {
                        // Must differ from left neighbor (down[idx-1]) if exists
                        if (idx > 0 && c == cur_down[idx - 1]) continue;
                        cur_up[idx] = c;
                        generate(pos + 1);
                    }
                } else {
                    // Downward triangle: index pos/2
                    int idx = pos / 2;
                    for (int c = 0; c < 3; c++) {
                        // Must differ from left neighbor (up[idx])
                        if (c == cur_up[idx]) continue;
                        // Must differ from right neighbor (up[idx+1]) - not placed yet
                        // Actually we handle that when placing up[idx+1]
                        // Must differ from prev_up[idx] (top neighbor)
                        if (c == prev_up[idx]) continue;
                        cur_down[idx] = c;
                        generate(pos + 1);
                    }
                }
            };

            generate(0);
        }

        dp = new_dp;
    }

    long long total = 0;
    for (auto& [state, cnt] : dp) {
        total += cnt;
    }

    cout << total << endl;
    return 0;
}
