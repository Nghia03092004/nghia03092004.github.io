// IOI 2004 - Artemis
// Find axis-aligned rectangle with max points strictly in interior.
// O(N^2 * M) via sweep on x-groups, Kadane on y for best interval.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    if (N <= 2) { cout << 0 << "\n"; return 0; }

    vector<int> px(N), py(N);
    for (int i = 0; i < N; i++) cin >> px[i] >> py[i];

    // Coordinate compression for y
    vector<int> ys = py;
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int M = (int)ys.size();

    vector<int> cy(N);
    for (int i = 0; i < N; i++)
        cy[i] = (int)(lower_bound(ys.begin(), ys.end(), py[i]) - ys.begin());

    // Sort points by x, group by same x
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return px[a] < px[b];
    });

    vector<vector<int>> groups;
    for (int i = 0; i < N; ) {
        int j = i;
        while (j < N && px[order[j]] == px[order[i]]) j++;
        vector<int> g;
        for (int k = i; k < j; k++) g.push_back(cy[order[k]]);
        groups.push_back(g);
        i = j;
    }

    int G = (int)groups.size();
    int ans = 0;

    // Enumerate left boundary (just right of column l).
    // Sweep right boundary; columns strictly between l and r are interior.
    for (int l = 0; l < G; l++) {
        vector<int> cnt(M, 0);
        for (int r = l + 1; r < G; r++) {
            // Column r-1 is now strictly inside if r-1 > l
            if (r - 1 > l) {
                for (int y : groups[r - 1]) cnt[y]++;
            }
            // Best y-interval: max contiguous subarray sum (Kadane)
            int best = 0, cur = 0;
            for (int y = 0; y < M; y++) {
                cur += cnt[y];
                best = max(best, cur);
                if (cur < 0) cur = 0;
            }
            ans = max(ans, best);
        }
    }

    cout << ans << "\n";
    return 0;
}
