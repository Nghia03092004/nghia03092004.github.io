#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<vector<long long>> S(R + 1, vector<long long>(C + 1, 0));
    for(int i = 1; i <= R; i++)
        for(int j = 1; j <= C; j++){
            int v; cin >> v;
            S[i][j] = v + S[i-1][j] + S[i][j-1] - S[i-1][j-1];
        }

    auto rectSum = [&](int r1, int c1, int r2, int c2) -> long long {
        if(r1 > r2 || c1 > c2) return 0;
        return S[r2][c2] - S[r1-1][c2] - S[r2][c1-1] + S[r1-1][c1-1];
    };

    int a, b, c, d; // outer a x b, inner c x d
    cin >> a >> b >> c >> d;

    // inner rectangle sum at position (i, j) = rectSum(i, j, i+c-1, j+d-1)
    // Precompute inner sums
    int innerRows = R - c + 1;
    int innerCols = C - d + 1;

    // For each outer rectangle at (r, c_pos):
    // Inner must have top-left in [r+1, r+a-c-1] x [c_pos+1, c_pos+b-d-1]
    // (1-based, inner fits strictly inside outer)

    // Height of inner window range: a - c - 1 rows
    // Width of inner window range: b - d - 1 columns
    int winH = a - c - 1;
    int winW = b - d - 1;

    if(winH < 0 || winW < 0){
        // Inner doesn't fit, just maximize outer
        long long ans = LLONG_MIN;
        for(int i = 1; i + a - 1 <= R; i++)
            for(int j = 1; j + b - 1 <= C; j++)
                ans = max(ans, rectSum(i, j, i+a-1, j+b-1));
        cout << ans << "\n";
        return 0;
    }

    // Compute inner sums for all valid positions
    vector<vector<long long>> innerS(R + 1, vector<long long>(C + 1, 0));
    for(int i = 1; i + c - 1 <= R; i++)
        for(int j = 1; j + d - 1 <= C; j++)
            innerS[i][j] = rectSum(i, j, i + c - 1, j + d - 1);

    // 2D sliding window minimum of innerS
    // Window size: (winH + 1) x (winW + 1)
    int wh = winH + 1, ww = winW + 1;

    // Step 1: row-wise sliding minimum
    vector<vector<long long>> rowMin(R + 1, vector<long long>(C + 1, LLONG_MAX));
    for(int i = 1; i + c - 1 <= R; i++){
        deque<int> dq;
        for(int j = 1; j + d - 1 <= C; j++){
            while(!dq.empty() && innerS[i][dq.back()] >= innerS[i][j])
                dq.pop_back();
            dq.push_back(j);
            if(dq.front() < j - ww + 1) dq.pop_front();
            if(j >= ww)
                rowMin[i][j - ww + 1] = innerS[i][dq.front()];
        }
    }

    // Step 2: column-wise sliding minimum on rowMin
    long long ans = LLONG_MIN;
    int maxOuterR = R - a + 1;
    int maxOuterC = C - b + 1;

    for(int j = 1; j <= maxOuterC; j++){
        deque<int> dq;
        // Column j in rowMin corresponds to inner starting column j+1
        // (since inner starts at outer_col + 1)
        // Actually, inner top-left ranges in [outer_r+1 .. outer_r+winH+1] row-wise
        // and [outer_c+1 .. outer_c+winW+1] col-wise.
        // For outer at (r, j), inner col starts from j+1, row from r+1.
        // rowMin[i][j+1] has the min over columns j+1 .. j+ww.
        // We need min over rows i = r+1 .. r+wh.

        for(int i = 1; i + c - 1 <= R; i++){
            long long val = rowMin[i][j + 1]; // might be out of bounds, check
            if(j + 1 > (int)rowMin[i].size() - 1) continue;
            while(!dq.empty() && rowMin[dq.back()][j+1] >= val)
                dq.pop_back();
            dq.push_back(i);
            if(dq.front() < i - wh + 1) dq.pop_front();
            if(i >= wh){
                int outerR = i - wh; // 0-indexed? Let's be careful
                // outer top-left row: inner must start from outerR+1
                // and inner row range is [outerR+1 .. outerR+wh]
                // so i ranges from outerR+1 to outerR+wh
                // i = outerR+wh => outerR = i - wh + 1 - 1 = i - wh
                // Hmm, let me just compute:
                int or_ = i - wh + 1 - 1; // outer row (1-based)
                if(or_ >= 1 && or_ + a - 1 <= R){
                    long long outerSum = rectSum(or_, j, or_ + a - 1, j + b - 1);
                    long long minInner = rowMin[dq.front()][j + 1];
                    ans = max(ans, outerSum - minInner);
                }
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
