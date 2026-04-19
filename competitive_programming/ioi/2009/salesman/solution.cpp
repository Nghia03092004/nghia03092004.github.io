// IOI 2009 - Salesman
// DP with segment trees for transitions. Process fairs grouped by time.
// Same-time fairs handled with left-right and right-left sweeps.
// O(M log N) time.
#include <bits/stdc++.h>
using namespace std;

const long long NEG_INF = -1e18;

struct SegTree {
    int n;
    vector<long long> tree;
    SegTree() : n(0) {}
    SegTree(int n) : n(n), tree(4 * n, NEG_INF) {}

    void update(int pos, long long val, int node, int lo, int hi) {
        if (lo == hi) {
            tree[node] = max(tree[node], val);
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(pos, val, 2 * node, lo, mid);
        else            update(pos, val, 2 * node + 1, mid + 1, hi);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }
    void update(int pos, long long val) {
        if (n > 0) update(pos, val, 1, 0, n - 1);
    }

    long long query(int l, int r, int node, int lo, int hi) {
        if (l > r || lo > r || hi < l) return NEG_INF;
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return max(query(l, r, 2 * node, lo, mid),
                   query(l, r, 2 * node + 1, mid + 1, hi));
    }
    long long query(int l, int r) {
        if (n == 0 || l > r) return NEG_INF;
        return query(l, r, 1, 0, n - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, U, D, S;
    cin >> N >> U >> D >> S;

    struct Fair { int t, pos, money; };
    int M;
    cin >> M;
    vector<Fair> fairs(M);
    for (int i = 0; i < M; i++) {
        cin >> fairs[i].t >> fairs[i].pos >> fairs[i].money;
    }

    // Sort by time, then by position.
    sort(fairs.begin(), fairs.end(), [](const Fair &a, const Fair &b) {
        return a.t != b.t ? a.t < b.t : a.pos < b.pos;
    });

    // Two segment trees indexed by position [0..N].
    // treeUp:   stores max(dp[j] + U * pos[j]) for positions <= p
    // treeDown: stores max(dp[j] - D * pos[j]) for positions >= p
    SegTree treeUp(N + 1), treeDown(N + 1);

    // Initialize from home position S.
    treeUp.update(S, (long long)U * S);
    treeDown.update(S, -(long long)D * S);

    vector<long long> dp(M);

    // Process fairs grouped by time.
    int i = 0;
    while (i < M) {
        int j = i;
        while (j < M && fairs[j].t == fairs[i].t) j++;

        // Compute dp from previous time groups.
        for (int k = i; k < j; k++) {
            int p = fairs[k].pos;
            long long best = NEG_INF;
            long long v1 = treeUp.query(0, p);
            if (v1 > NEG_INF) best = max(best, v1 - (long long)U * p);
            long long v2 = treeDown.query(p, N);
            if (v2 > NEG_INF) best = max(best, v2 + (long long)D * p);
            dp[k] = best + fairs[k].money;
        }

        // Left-to-right sweep (upstream, cost U per unit).
        {
            long long bestVal = NEG_INF;
            for (int k = i; k < j; k++) {
                int p = fairs[k].pos;
                if (bestVal > NEG_INF) {
                    dp[k] = max(dp[k], bestVal - (long long)U * p + fairs[k].money);
                }
                bestVal = max(bestVal, dp[k] + (long long)U * p);
            }
        }

        // Right-to-left sweep (downstream, cost D per unit).
        {
            long long bestVal = NEG_INF;
            for (int k = j - 1; k >= i; k--) {
                int p = fairs[k].pos;
                if (bestVal > NEG_INF) {
                    dp[k] = max(dp[k], bestVal + (long long)D * p + fairs[k].money);
                }
                bestVal = max(bestVal, dp[k] - (long long)D * p);
            }
        }

        // Extra left-to-right pass to propagate right-to-left updates.
        {
            long long bestVal = NEG_INF;
            for (int k = i; k < j; k++) {
                int p = fairs[k].pos;
                if (bestVal > NEG_INF) {
                    dp[k] = max(dp[k], bestVal - (long long)U * p + fairs[k].money);
                }
                bestVal = max(bestVal, dp[k] + (long long)U * p);
            }
        }

        // Update segment trees.
        for (int k = i; k < j; k++) {
            int p = fairs[k].pos;
            treeUp.update(p, dp[k] + (long long)U * p);
            treeDown.update(p, dp[k] - (long long)D * p);
        }

        i = j;
    }

    // Best answer: max dp[k] minus cost to return home.
    long long ans = 0; // attending no fairs is valid
    for (int k = 0; k < M; k++) {
        int p = fairs[k].pos;
        long long costHome = (p <= S) ? (long long)U * (S - p)
                                      : (long long)D * (p - S);
        ans = max(ans, dp[k] - costHome);
    }

    cout << ans << "\n";
    return 0;
}
