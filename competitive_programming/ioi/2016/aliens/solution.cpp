#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * IOI 2016 - Aliens
 *
 * Cover N points with at most K diagonal-aligned squares, minimize total area.
 *
 * 1) Transform (r,c) -> interval [min(r,c), max(r,c)+1]
 * 2) Remove dominated intervals
 * 3) Alien's trick (Lagrangian relaxation) on number of squares
 * 4) For fixed penalty lambda, DP + Convex Hull Trick in O(n)
 *
 * dp[i] = min over 0<=p<i of { dp[p] + (U[i-1] - L[p])^2 - OV[p]^2 + lambda }
 *   where OV[p] = max(0, U[p-1] - L[p]) for p>=1, OV[0]=0
 *
 * CHT form: for transition from p, define line with
 *   slope = -2*L[p], intercept = dp[p] + L[p]^2 - OV[p]^2
 *   query at x = U[i-1], then dp[i] = result + x^2 + lambda
 */

struct Point { ll l, u; };

struct Line {
    ll m, b;
    int cnt;
    ll eval(ll x) const { return m * x + b; }
};

// Monotone CHT (lines added with decreasing slopes, queries with increasing x)
struct CHT {
    vector<Line> hull;
    int ptr;

    CHT() : ptr(0) {}

    bool bad(const Line &l1, const Line &l2, const Line &l3) {
        // l2 is redundant if intersection of l1,l3 is <= intersection of l1,l2
        // (l3.b - l1.b) * (l1.m - l2.m) <= (l2.b - l1.b) * (l1.m - l3.m)
        return (lll)(l3.b - l1.b) * (l1.m - l2.m) <=
               (lll)(l2.b - l1.b) * (l1.m - l3.m);
    }

    void addLine(Line l) {
        while ((int)hull.size() >= 2 &&
               bad(hull[hull.size()-2], hull[hull.size()-1], l))
            hull.pop_back();
        hull.push_back(l);
    }

    pair<ll, int> query(ll x) {
        // pointer-based query for increasing x
        if (ptr >= (int)hull.size()) ptr = (int)hull.size() - 1;
        while (ptr + 1 < (int)hull.size() &&
               hull[ptr+1].eval(x) <= hull[ptr].eval(x))
            ptr++;
        return {hull[ptr].eval(x), hull[ptr].cnt};
    }
};

static vector<Point> pts;

// solve(lambda) returns {dp_value, count_of_squares}
// dp_value already includes count * lambda
pair<ll, int> solve(ll lambda) {
    int sz = (int)pts.size();

    // OV[i] = max(0, U[i-1] - L[i]) for i>=1, OV[0] = 0
    vector<ll> ov(sz + 1, 0);
    for (int i = 1; i < sz; i++) {
        ll overlap = pts[i-1].u - pts[i].l;
        ov[i] = max(0LL, overlap);
    }

    vector<ll> dp(sz + 1, 0);
    vector<int> cnt(sz + 1, 0);

    CHT cht;

    // Add line for p=0: slope = -2*L[0], intercept = dp[0] + L[0]^2 - OV[0]^2
    // OV[0] = 0, dp[0] = 0
    cht.addLine({-2 * pts[0].l, pts[0].l * pts[0].l, 0});

    for (int i = 1; i <= sz; i++) {
        ll x = pts[i-1].u;
        auto [val, c] = cht.query(x);
        dp[i] = val + x * x + lambda;
        cnt[i] = c + 1;

        if (i < sz) {
            ll M = -2 * pts[i].l;
            ll B = dp[i] + pts[i].l * pts[i].l - ov[i] * ov[i];
            cht.addLine({M, B, cnt[i]});
        }
    }

    return {dp[sz], cnt[sz]};
}

long long take_photos(int N, int M, int K, int r[], int c[]) {
    // Step 1: transform to intervals
    vector<Point> raw(N);
    for (int i = 0; i < N; i++) {
        ll lo = min(r[i], c[i]);
        ll hi = max(r[i], c[i]) + 1;
        raw[i] = {lo, hi};
    }

    // Step 2: sort by left endpoint, break ties by larger right endpoint first
    sort(raw.begin(), raw.end(), [](const Point &a, const Point &b) {
        return a.l < b.l || (a.l == b.l && a.u > b.u);
    });

    // Remove dominated intervals: after sorting, keep only intervals where
    // u is strictly increasing (since l is non-decreasing)
    pts.clear();
    for (auto &p : raw) {
        // Remove intervals that are fully contained in p (same l, smaller u)
        // and intervals where p is contained (already handled by u check)
        while (!pts.empty() && pts.back().u <= p.u && pts.back().l >= p.l)
            pts.pop_back();
        if (pts.empty() || p.u > pts.back().u)
            pts.push_back(p);
    }

    int sz = (int)pts.size();
    if (K >= sz) {
        // Can use one square per interval, just sum individual areas minus overlaps
        // Actually: each interval [l,u] has area (u-l)^2, and consecutive
        // squares may overlap. Best is one square per interval.
        ll ans = 0;
        for (int i = 0; i < sz; i++) {
            ll side = pts[i].u - pts[i].l;
            ans += side * side;
            if (i > 0) {
                ll overlap = max(0LL, pts[i-1].u - pts[i].l);
                ans -= overlap * overlap;
            }
        }
        return ans;
    }

    // Step 3: Alien's trick - binary search on lambda
    // Higher lambda penalizes more squares, so count decreases as lambda increases.
    // We want count <= K.
    ll lo = 0, hi = (ll)M * M;
    ll best_val = 0;
    int best_cnt = 0;

    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        auto [val, c] = solve(mid);
        if (c <= K) {
            // lambda might be too large (or just right), try smaller
            best_val = val;
            best_cnt = c;
            hi = mid - 1;
        } else {
            // too many squares, increase lambda
            lo = mid + 1;
        }
    }

    // At this point, lo is the smallest lambda where count <= K.
    // The answer is: dp_value - K * lambda
    // where dp_value = best_val was computed with lambda = lo
    // But best_val was computed at hi+1 = lo? Let's recompute at lo to be safe.
    auto [final_val, final_cnt] = solve(lo);

    // answer = val - K * lo
    // val already includes final_cnt * lo in it, and we want the original cost
    // for exactly K squares. The Alien's trick gives:
    //   f(K) = g(lambda) - K * lambda
    // where g(lambda) = min over all cnt of (cost + cnt * lambda)
    return final_val - (ll)K * lo;
}

int main() {
    int N, M, K;
    scanf("%d %d %d", &N, &M, &K);
    vector<int> r(N), c(N);
    for (int i = 0; i < N; i++) scanf("%d %d", &r[i], &c[i]);
    printf("%lld\n", take_photos(N, M, K, r.data(), c.data()));
    return 0;
}
