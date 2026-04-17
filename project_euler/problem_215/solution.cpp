#include <bits/stdc++.h>
using namespace std;

int W = 32;
int H = 10;

vector<vector<int>> rows; // each row is a set of crack positions

void generate(int pos, vector<int>& cracks) {
    if (pos == W) {
        rows.push_back(cracks);
        return;
    }
    // Try placing a 2-brick
    if (pos + 2 <= W) {
        if (pos + 2 < W) cracks.push_back(pos + 2);
        generate(pos + 2, cracks);
        if (pos + 2 < W) cracks.pop_back();
    }
    // Try placing a 3-brick
    if (pos + 3 <= W) {
        if (pos + 3 < W) cracks.push_back(pos + 3);
        generate(pos + 3, cracks);
        if (pos + 3 < W) cracks.pop_back();
    }
}

int main() {
    vector<int> cracks;
    generate(0, cracks);

    int m = rows.size();

    // Convert crack positions to bitmask for fast compatibility check
    vector<unsigned int> mask(m, 0);
    for (int i = 0; i < m; i++) {
        for (int c : rows[i]) {
            mask[i] |= (1u << c);
        }
    }

    // Build compatibility list
    vector<vector<int>> compat(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if ((mask[i] & mask[j]) == 0) {
                compat[i].push_back(j);
            }
        }
    }

    // DP
    vector<long long> dp(m, 1); // height 1: each row has 1 way

    for (int h = 2; h <= H; h++) {
        vector<long long> ndp(m, 0);
        for (int i = 0; i < m; i++) {
            for (int j : compat[i]) {
                ndp[i] += dp[j];
            }
        }
        dp = ndp;
    }

    long long answer = 0;
    for (int i = 0; i < m; i++) {
        answer += dp[i];
    }

    cout << answer << endl;
    return 0;
}
