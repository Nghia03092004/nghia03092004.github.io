#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2021 - Candies
// n boxes with capacities c[i]. q range-update operations add v[j] to boxes
// in [l[j],r[j]], clamped to [0, c[i]]. Find final candy counts.
//
// Approach: sweep boxes left to right. Maintain a lazy segment tree over
// operations representing prefix sums of active operations. For each box,
// binary search for the latest "capping event" using suffix max/min queries.

vector<int> distribute_candies(vector<int> c, vector<int> l,
                               vector<int> r, vector<int> v) {
    int n = (int)c.size(), q = (int)l.size();
    vector<int> ans(n);

    // Events: operation j starts at l[j], ends after r[j]
    vector<vector<int>> starts(n), ends(n);
    for (int j = 0; j < q; j++) {
        starts[l[j]].push_back(j);
        if (r[j] + 1 < n) ends[r[j] + 1].push_back(j);
    }

    // Lazy segment tree with range-add, range max/min query, point query
    struct LazySegTree {
        int n;
        vector<ll> mx, mn, lazy;

        LazySegTree(int n) : n(n), mx(4 * (n + 1), 0),
            mn(4 * (n + 1), 0), lazy(4 * (n + 1), 0) {}

        void push_down(int node) {
            if (lazy[node] != 0) {
                for (int c : {2 * node, 2 * node + 1}) {
                    mx[c] += lazy[node];
                    mn[c] += lazy[node];
                    lazy[c] += lazy[node];
                }
                lazy[node] = 0;
            }
        }

        void range_add(int l, int r, ll val, int node, int lo, int hi) {
            if (l > hi || r < lo) return;
            if (l <= lo && hi <= r) {
                mx[node] += val;
                mn[node] += val;
                lazy[node] += val;
                return;
            }
            push_down(node);
            int mid = (lo + hi) / 2;
            range_add(l, r, val, 2 * node, lo, mid);
            range_add(l, r, val, 2 * node + 1, mid + 1, hi);
            mx[node] = max(mx[2 * node], mx[2 * node + 1]);
            mn[node] = min(mn[2 * node], mn[2 * node + 1]);
        }
        void range_add(int l, int r, ll val) {
            range_add(l, r, val, 1, 0, n);
        }

        ll qmax(int l, int r, int node, int lo, int hi) {
            if (l > hi || r < lo) return LLONG_MIN;
            if (l <= lo && hi <= r) return mx[node];
            push_down(node);
            int mid = (lo + hi) / 2;
            return max(qmax(l, r, 2 * node, lo, mid),
                       qmax(l, r, 2 * node + 1, mid + 1, hi));
        }
        ll qmax(int l, int r) { return qmax(l, r, 1, 0, n); }

        ll qmin(int l, int r, int node, int lo, int hi) {
            if (l > hi || r < lo) return LLONG_MAX;
            if (l <= lo && hi <= r) return mn[node];
            push_down(node);
            int mid = (lo + hi) / 2;
            return min(qmin(l, r, 2 * node, lo, mid),
                       qmin(l, r, 2 * node + 1, mid + 1, hi));
        }
        ll qmin(int l, int r) { return qmin(l, r, 1, 0, n); }

        // Find latest j in [0, n] where max(S[j..n]) - min(S[j..n]) > cap
        int find_latest(ll cap) {
            int lo = 0, hi = n, result = -1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                ll mx_val = qmax(mid, n);
                ll mn_val = qmin(mid, n);
                if (mx_val - mn_val > cap) {
                    result = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            return result;
        }

        ll point_query(int pos, int node, int lo, int hi) {
            if (lo == hi) return mx[node];
            push_down(node);
            int mid = (lo + hi) / 2;
            if (pos <= mid) return point_query(pos, 2 * node, lo, mid);
            else return point_query(pos, 2 * node + 1, mid + 1, hi);
        }
        ll point_query(int pos) { return point_query(pos, 1, 0, n); }
    };

    LazySegTree seg(q);

    for (int i = 0; i < n; i++) {
        // Activate operations starting at box i
        for (int j : starts[i])
            seg.range_add(j + 1, q, v[j]);
        // Deactivate operations ending before box i
        for (int j : ends[i])
            seg.range_add(j + 1, q, -v[j]);

        ll cap = c[i];
        int j = seg.find_latest(cap);
        ll S_end = seg.point_query(q);

        if (j == -1) {
            // No capping event: clamp total sum to [0, cap]
            ans[i] = (int)max(0LL, min((ll)cap, S_end));
        } else {
            // Capping event at position j
            ll S_j = seg.point_query(j);
            ll mx_after = (j + 1 <= q) ? seg.qmax(j + 1, q) : LLONG_MIN;
            ll mn_after = (j + 1 <= q) ? seg.qmin(j + 1, q) : LLONG_MAX;

            if (S_j >= mx_after) {
                // S[j] was the max: box capped at full
                ans[i] = (int)(cap - (S_j - S_end));
            } else {
                // S[j] was the min: box capped at 0
                ans[i] = (int)(S_end - S_j);
            }
            ans[i] = max(0, min((int)cap, ans[i]));
        }
    }

    return ans;
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    vector<int> c(n);
    for (int i = 0; i < n; i++) scanf("%d", &c[i]);
    vector<int> l(q), r(q), v(q);
    for (int j = 0; j < q; j++) scanf("%d %d %d", &l[j], &r[j], &v[j]);
    auto ans = distribute_candies(c, l, r, v);
    for (int i = 0; i < n; i++)
        printf("%d%c", ans[i], " \n"[i == n - 1]);
    return 0;
}
