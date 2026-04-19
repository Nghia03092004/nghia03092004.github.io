// IOI 2005 - Garden
// Largest rectangle of free cells in an R x C grid.
// Histogram approach with stack-based largest rectangle per row, O(R*C).
#include <bits/stdc++.h>
using namespace std;

int largestRectInHistogram(vector<int>& h) {
    int n = (int)h.size();
    stack<int> st;
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        int cur = (i == n) ? 0 : h[i];
        while (!st.empty() && h[st.top()] >= cur) {
            int height = h[st.top()];
            st.pop();
            int width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    return maxArea;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<vector<int>> grid(R, vector<int>(C));
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            cin >> grid[i][j];

    // Build height array row by row; find max rectangle in histogram
    vector<int> h(C, 0);
    int ans = 0;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            h[j] = (grid[i][j] == 0) ? h[j] + 1 : 0;
        }
        ans = max(ans, largestRectInHistogram(h));
    }

    cout << ans << "\n";
    return 0;
}
