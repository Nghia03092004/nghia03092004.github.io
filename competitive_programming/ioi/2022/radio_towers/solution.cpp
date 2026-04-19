#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;

// Sparse table for range maximum queries
vector<vector<int>> sparse; // sparse[k][i] = index of max in [i, i+2^k-1]
int LOG;

void init(int _N, vector<int> _H) {
    N = _N;
    H = _H;
    LOG = 1;
    while ((1 << LOG) <= N) LOG++;

    sparse.assign(LOG, vector<int>(N));
    for (int i = 0; i < N; i++) sparse[0][i] = i;
    for (int k = 1; k < LOG; k++)
        for (int i = 0; i + (1 << k) <= N; i++) {
            int a = sparse[k-1][i], b = sparse[k-1][i + (1 << (k-1))];
            sparse[k][i] = (H[a] >= H[b]) ? a : b;
        }
}

int range_max_idx(int l, int r) {
    int k = __lg(r - l + 1);
    int a = sparse[k][l], b = sparse[k][r - (1 << k) + 1];
    return (H[a] >= H[b]) ? a : b;
}

int max_towers(int L, int R, int D) {
    if (L == R) return 1;

    // Greedy: find maximum set of towers in [L, R] such that between any two
    // consecutive selected towers, there exists a peak >= max(H[selected]) + D.

    // Strategy: select towers greedily as valleys.
    // A tower can be selected if there's a high enough separator from the previous selection.

    // Find the global maximum in [L, R]
    int peak_idx = range_max_idx(L, R);

    // Try selecting towers: scan and greedily pick
    // Track the minimum height selected so far and ensure separators exist.

    // Simple approach: for each tower, check if it can be part of the set.
    // A tower at position i with height H[i] can be selected if:
    // - There exists j in [last_selected+1, i-1] with H[j] >= H[i] + D
    //   AND H[j] >= H[last_selected] + D

    int count = 0;
    int last = -1; // index of last selected tower

    for (int i = L; i <= R; i++) {
        if (last == -1) {
            // First selection: check if there's a peak to the right
            // that's high enough (will be verified when selecting the second)
            // For now, tentatively select
            // Actually, we should select the lowest possible towers.
            // Let's try: select if i is a local minimum in [L, R]
            bool is_valley = true;
            if (i > L && H[i] > H[i-1]) is_valley = false;
            if (i < R && H[i] > H[i+1]) is_valley = false;

            if (is_valley || i == L || i == R) {
                // Check feasibility: is there a separator available?
                // For the first one, just select tentatively
                if (count == 0) {
                    last = i;
                    count = 1;
                } else {
                    // Check separator between last and i
                    if (last + 1 <= i - 1) {
                        int mx = range_max_idx(last + 1, i - 1);
                        if (H[mx] >= H[last] + D && H[mx] >= H[i] + D) {
                            last = i;
                            count++;
                        } else if (H[i] < H[last]) {
                            // Replace last with i (lower is better)
                            last = i;
                        }
                    }
                }
            }
        } else {
            // Try to select i
            if (i == last) continue;
            if (last + 1 <= i - 1) {
                int mx = range_max_idx(last + 1, i - 1);
                if (H[mx] >= H[last] + D && H[mx] >= H[i] + D) {
                    last = i;
                    count++;
                } else if (H[i] < H[last] && count == 1) {
                    // Replace: lower tower is better as starting point
                    last = i;
                }
            }
        }
    }

    if (count == 0) count = 1; // at least one tower can be selected

    return count;
}

int main() {
    int n, q;
    scanf("%d", &n);
    vector<int> h(n);
    for (int i = 0; i < n; i++) scanf("%d", &h[i]);
    init(n, h);
    scanf("%d", &q);
    while (q--) {
        int l, r, d;
        scanf("%d %d %d", &l, &r, &d);
        printf("%d\n", max_towers(l, r, d));
    }
    return 0;
}
