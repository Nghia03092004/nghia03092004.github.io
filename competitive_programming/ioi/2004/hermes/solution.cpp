// IOI 2004 - Hermes
// Visit N events on two axes in order. Cost = Chebyshev distance.
// O(N) interval-tracking DP on the free axis.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    if (N == 0) { cout << 0 << "\n"; return 0; }

    vector<int> d(N), p(N);
    for (int i = 0; i < N; i++) {
        cin >> d[i] >> p[i];
        d[i]--; // 0-indexed: 0 = x-axis, 1 = y-axis
    }

    // Track interval [lo, hi] of reachable free-axis positions at min cost
    long long cost = abs(p[0]);
    long long lo = -abs(p[0]);
    long long hi = abs(p[0]);

    for (int i = 1; i < N; i++) {
        if (d[i] == d[i - 1]) {
            // Same axis: mandatory travel, free axis can drift
            long long dt = abs(p[i] - p[i - 1]);
            cost += dt;
            lo -= dt;
            hi += dt;
        } else {
            // Cross axis: free axis was [lo,hi], must reach p[i] on it
            long long dist;
            if (p[i] >= lo && p[i] <= hi)
                dist = 0;
            else if (p[i] < lo)
                dist = lo - p[i];
            else
                dist = p[i] - hi;

            cost += dist;
            // New free axis centered at p[i-1] with budget dist
            lo = (long long)p[i - 1] - dist;
            hi = (long long)p[i - 1] + dist;
        }
    }

    cout << cost << "\n";
    return 0;
}
