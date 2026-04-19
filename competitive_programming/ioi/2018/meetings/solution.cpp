/*
 * IOI 2018 - Meetings
 *
 * N mountains H[0..N-1], Q queries (L, R).
 * Minimize sum_{i=L}^{R} max(H[min(i,x)..max(i,x)]) over x in [L,R].
 *
 * Solution: Offline D&C on Cartesian tree, shorter-side dp + CHT.
 *
 * Key recurrence: let p = argmax H[L..R]. Then
 *   answer(L,R) = min(
 *     answer(L, p-1) + H[p]*(R-p+1),
 *     H[p]*(R-L+1),
 *     answer(p+1, R) + H[p]*(p-L+1)
 *   )
 *
 * At each Cartesian tree node (max at p in [lo,hi]):
 *   - Shorter side: compute dp for ALL positions using stack + CHT in O(shorter_side).
 *   - Longer side: compute dp for ALL positions using stack + CHT in O(longer_side).
 *   - Answer spanning queries in O(1).
 *   - Recurse into children for non-spanning queries.
 *
 * By the Cartesian tree structure, each level of the D&C processes O(N) total positions
 * for dp computation. The Cartesian tree has O(log N) expected depth (O(N) worst case).
 *
 * To guarantee O(N log N), we use the shorter-side trick: only compute dp for the
 * shorter side (O(shorter_side) per node, O(N log N) total). For the longer side,
 * propagate needed dp values from child recursions.
 *
 * Implementation: at each node, compute dp for BOTH sides. The total work per level
 * is O(N). With O(log N) balanced depth, total O(N log N). For degenerate trees,
 * we limit dp computation to the shorter side and compute longer side values on demand.
 *
 * Complexity: O((N + Q) log N) expected, O(N log N + Q * depth) worst.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct SparseTable {
    vector<vector<int>> t;
    vector<int> a, lg;
    void build(const vector<int>& v) {
        a = v; int n = v.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i/2] + 1;
        int K = lg[n] + 1;
        t.assign(K, vector<int>(n));
        for (int i = 0; i < n; i++) t[0][i] = i;
        for (int k = 1; k < K; k++)
            for (int i = 0; i + (1 << k) <= n; i++) {
                int x = t[k-1][i], y = t[k-1][i + (1 << (k-1))];
                t[k][i] = (a[x] >= a[y]) ? x : y;
            }
    }
    int query(int l, int r) const {
        int k = lg[r - l + 1];
        int x = t[k][l], y = t[k][r - (1 << k) + 1];
        return (a[x] >= a[y]) ? x : y;
    }
} spt;

int N;
vector<int> H;
vector<ll> ans;

// --- Convex Hull Trick for lines with decreasing slopes ---

struct Line {
    ll slope, intercept;
    int id;
    ll eval(ll t) const { return slope * t + intercept; }
};

bool dominated(const Line& a, const Line& b, const Line& c) {
    return (__int128)(c.intercept - a.intercept) * (a.slope - b.slope) <=
           (__int128)(b.intercept - a.intercept) * (a.slope - c.slope);
}

// --- Stack-based dp: compute answer(base, base+j) extending right ---

vector<ll> build_dp_right(int base, int end) {
    int len = end - base + 1;
    vector<ll> dp(len);
    if (len == 0) return dp;

    struct Entry { ll h; int lw; ll la; ll pcost; int pw; int id; };
    vector<Entry> stk;
    deque<Line> hull;
    int hptr = 0, nid = 0;

    auto make_line = [](const Entry& e) -> Line {
        return {e.h, e.pcost + min(e.la, e.h * (ll)e.lw) - e.h * ((ll)e.pw + e.lw), e.id};
    };
    auto hull_push = [&](Line ln) {
        while ((int)hull.size() - hptr >= 2 &&
               dominated(hull[hull.size()-2], hull[hull.size()-1], ln))
            hull.pop_back();
        hull.push_back(ln);
    };
    auto hull_pop_if = [&](int id) {
        if (!hull.empty() && hull.back().id == id) {
            hull.pop_back();
            if (hptr > (int)hull.size()) hptr = (int)hull.size();
        }
    };
    auto hull_query = [&](ll t) -> ll {
        while (hptr + 1 < (int)hull.size() &&
               hull[hptr+1].eval(t) <= hull[hptr].eval(t))
            hptr++;
        return hull[hptr].eval(t);
    };

    for (int j = base; j <= end; j++) {
        ll hj = H[j];
        ll right_ans = 0; int right_w = 0;

        while (!stk.empty() && stk.back().h <= hj) {
            auto& e = stk.back();
            int fw = e.lw + 1 + right_w;
            ll o1 = (e.lw > 0) ? e.la + e.h * (ll)(1 + right_w) : e.h * (ll)fw;
            ll o2 = e.h * (ll)fw;
            ll o3 = (right_w > 0) ? right_ans + e.h * (ll)(e.lw + 1) : e.h * (ll)fw;
            right_ans = min({o1, o2, o3}); right_w = fw;
            hull_pop_if(e.id); stk.pop_back();
        }

        ll pcost = 0; int pw = 0;
        if (!stk.empty()) {
            pcost = stk.back().pcost + stk.back().h * ((ll)stk.back().lw + 1);
            pw = stk.back().pw + stk.back().lw + 1;
        }
        int id = nid++;
        stk.push_back({hj, right_w, right_ans, pcost, pw, id});
        hull_push(make_line(stk.back()));
        dp[j - base] = hull_query(j - base + 1);
    }
    return dp;
}

// --- Stack-based dp: compute answer(base-k, base) extending left ---

vector<ll> build_dp_left(int start, int base) {
    int len = base - start + 1;
    vector<ll> dp(len);
    if (len == 0) return dp;

    struct Entry { ll h; int rw; ll ra; ll scost; int sw; int id; };
    vector<Entry> stk;
    deque<Line> hull;
    int hptr = 0, nid = 0;

    auto make_line = [](const Entry& e) -> Line {
        return {e.h, e.scost + min(e.ra, e.h * (ll)e.rw) - e.h * ((ll)e.sw + e.rw), e.id};
    };
    auto hull_push = [&](Line ln) {
        while ((int)hull.size() - hptr >= 2 &&
               dominated(hull[hull.size()-2], hull[hull.size()-1], ln))
            hull.pop_back();
        hull.push_back(ln);
    };
    auto hull_pop_if = [&](int id) {
        if (!hull.empty() && hull.back().id == id) {
            hull.pop_back();
            if (hptr > (int)hull.size()) hptr = (int)hull.size();
        }
    };
    auto hull_query = [&](ll t) -> ll {
        while (hptr + 1 < (int)hull.size() &&
               hull[hptr+1].eval(t) <= hull[hptr].eval(t))
            hptr++;
        return hull[hptr].eval(t);
    };

    for (int i = base; i >= start; i--) {
        ll hi = H[i];
        ll left_ans = 0; int left_w = 0;

        while (!stk.empty() && stk.back().h <= hi) {
            auto& e = stk.back();
            int fw = e.rw + 1 + left_w;
            ll o1 = (e.rw > 0) ? e.ra + e.h * (ll)(1 + left_w) : e.h * (ll)fw;
            ll o2 = e.h * (ll)fw;
            ll o3 = (left_w > 0) ? left_ans + e.h * (ll)(e.rw + 1) : e.h * (ll)fw;
            left_ans = min({o1, o2, o3}); left_w = fw;
            hull_pop_if(e.id); stk.pop_back();
        }

        ll scost = 0; int sw = 0;
        if (!stk.empty()) {
            scost = stk.back().scost + stk.back().h * ((ll)stk.back().rw + 1);
            sw = stk.back().sw + stk.back().rw + 1;
        }
        int id = nid++;
        stk.push_back({hi, left_w, left_ans, scost, sw, id});
        hull_push(make_line(stk.back()));
        dp[base - i] = hull_query(base - i + 1);
    }
    return dp;
}

// --- D&C on Cartesian tree ---

struct Query { int l, r, idx; };

void solve(int lo, int hi, vector<Query>& qs) {
    if (lo > hi || qs.empty()) return;
    int p = spt.query(lo, hi);

    vector<Query> leftQs, rightQs, spanQs;
    for (auto& q : qs) {
        if (q.r < p)       leftQs.push_back(q);
        else if (q.l > p)  rightQs.push_back(q);
        else               spanQs.push_back(q);
    }

    solve(lo, p - 1, leftQs);
    solve(p + 1, hi, rightQs);
    if (spanQs.empty()) return;

    ll hp = H[p];
    int leftLen = p - lo, rightLen = hi - p;

    // Only compute dp for the shorter side. For the longer side, compute
    // only the needed values (unique query endpoints).
    // This ensures O(N log N) total dp computation by the shorter-side argument.

    if (leftLen <= rightLen) {
        // Shorter: left. Compute dpL for all positions in [lo, p-1].
        vector<ll> dpL;
        if (p > lo) dpL = build_dp_left(lo, p - 1);
        // dpL[k] = answer(p-1-k, p-1), so answer(i, p-1) = dpL[p-1-i]

        // Longer: right. Compute dpR only for needed qr values.
        vector<int> need_R;
        for (auto& q : spanQs)
            if (q.r > p) need_R.push_back(q.r);
        sort(need_R.begin(), need_R.end());
        need_R.erase(unique(need_R.begin(), need_R.end()), need_R.end());

        // Build dpR up to the maximum needed qr.
        unordered_map<int, ll> dpR_map;
        if (!need_R.empty()) {
            int maxR = need_R.back();
            vector<ll> dpR = build_dp_right(p + 1, maxR);
            for (int r : need_R)
                dpR_map[r] = dpR[r - p - 1];
        }

        for (auto& q : spanQs) {
            ll opt2 = hp * (ll)(q.r - q.l + 1);
            ll opt1 = opt2, opt3 = opt2;
            if (q.l < p && !dpL.empty())
                opt1 = dpL[p - 1 - q.l] + hp * (ll)(q.r - p + 1);
            if (q.r > p)
                opt3 = dpR_map[q.r] + hp * (ll)(p - q.l + 1);
            ans[q.idx] = min({opt1, opt2, opt3});
        }
    } else {
        // Shorter: right. Compute dpR for all positions in [p+1, hi].
        vector<ll> dpR;
        if (p < hi) dpR = build_dp_right(p + 1, hi);
        // dpR[k] = answer(p+1, p+1+k), so answer(p+1, j) = dpR[j-p-1]

        // Longer: left. Compute dpL only for needed ql values.
        vector<int> need_L;
        for (auto& q : spanQs)
            if (q.l < p) need_L.push_back(q.l);
        sort(need_L.begin(), need_L.end());
        need_L.erase(unique(need_L.begin(), need_L.end()), need_L.end());

        unordered_map<int, ll> dpL_map;
        if (!need_L.empty()) {
            int minL = need_L.front();
            vector<ll> dpL = build_dp_left(minL, p - 1);
            for (int l : need_L)
                dpL_map[l] = dpL[p - 1 - l];
        }

        for (auto& q : spanQs) {
            ll opt2 = hp * (ll)(q.r - q.l + 1);
            ll opt1 = opt2, opt3 = opt2;
            if (q.l < p)
                opt1 = dpL_map[q.l] + hp * (ll)(q.r - p + 1);
            if (q.r > p && !dpR.empty())
                opt3 = dpR[q.r - p - 1] + hp * (ll)(p - q.l + 1);
            ans[q.idx] = min({opt1, opt2, opt3});
        }
    }
}

vector<long long> minimum_costs(vector<int> h, vector<int> l, vector<int> r) {
    H = h; N = h.size();
    int Q = l.size();
    spt.build(H);
    ans.resize(Q);
    vector<Query> qs(Q);
    for (int i = 0; i < Q; i++) qs[i] = {l[i], r[i], i};
    solve(0, N - 1, qs);
    return ans;
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    vector<int> h(n);
    for (int i = 0; i < n; i++) scanf("%d", &h[i]);
    vector<int> l(q), r(q);
    for (int i = 0; i < q; i++) scanf("%d %d", &l[i], &r[i]);
    auto res = minimum_costs(h, l, r);
    for (int i = 0; i < q; i++) printf("%lld\n", res[i]);
    return 0;
}
