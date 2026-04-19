#include <bits/stdc++.h>
using namespace std;

using ll = long long;

long long min_total_length(vector<int> r, vector<int> b) {
    vector<pair<int, int>> pts;
    pts.reserve(r.size() + b.size());
    for (int x : r) pts.push_back({x, 0});
    for (int x : b) pts.push_back({x, 1});
    sort(pts.begin(), pts.end());

    int n = (int)pts.size();
    vector<pair<int, int>> groups;
    for (int i = 0; i < n;) {
        int j = i;
        while (j < n && pts[j].second == pts[i].second) ++j;
        groups.push_back({i, j - 1});
        i = j;
    }

    int g = (int)groups.size();
    vector<int> block_size(g);
    ll base = 0;

    const ll NEG_INF = -(1LL << 60);
    const ll POS_INF = (1LL << 60);

    for (int id = 0; id < g; ++id) {
        auto [l, rr] = groups[id];
        block_size[id] = rr - l + 1;
        ll prev = (id > 0) ? pts[groups[id - 1].second].first : NEG_INF;
        ll next = (id + 1 < g) ? pts[groups[id + 1].first].first : POS_INF;
        for (int i = l; i <= rr; ++i) {
            ll best = POS_INF;
            if (prev != NEG_INF) best = min(best, (ll)pts[i].first - prev);
            if (next != POS_INF) best = min(best, next - (ll)pts[i].first);
            base += best;
        }
    }

    vector<ll> dp_prev(1, 0);
    for (int id = 0; id + 1 < g; ++id) {
        auto [l1, r1] = groups[id];
        auto [l2, r2] = groups[id + 1];
        int p = r1 - l1 + 1;
        int q = r2 - l2 + 1;

        ll u = (id > 0) ? pts[groups[id - 1].second].first : NEG_INF;
        ll v = (id + 2 < g) ? pts[groups[id + 2].first].first : POS_INF;
        ll a_last = pts[r1].first;
        ll b_first = pts[l2].first;
        ll gap = b_first - a_last;

        vector<ll> left_vals(p), right_vals(q);
        for (int i = 0; i < p; ++i) {
            ll cur = 2LL * pts[l1 + i].first - (u + b_first);
            left_vals[i] = min(0LL, cur);
        }
        for (int i = 0; i < q; ++i) {
            ll cur = (v + a_last) - 2LL * pts[l2 + i].first;
            right_vals[i] = min(0LL, cur);
        }

        vector<ll> best_left(p + 1, 0), best_right(q + 1, 0);
        for (int t = 1; t <= p; ++t) best_left[t] = best_left[t - 1] + left_vals[p - t];
        for (int t = 1; t <= q; ++t) best_right[t] = best_right[t - 1] + right_vals[t - 1];

        int cap = min(p, q);
        vector<ll> profit(cap + 1, 0);
        for (int t = 1; t <= cap; ++t) {
            profit[t] = t * gap + best_left[t] + best_right[t];
        }

        vector<ll> prefix_best(dp_prev.size());
        ll cur_best = NEG_INF;
        for (int i = 0; i < (int)dp_prev.size(); ++i) {
            cur_best = max(cur_best, dp_prev[i]);
            prefix_best[i] = cur_best;
        }

        vector<ll> dp_cur(cap + 1, NEG_INF);
        for (int t = 0; t <= cap; ++t) {
            int lim = min(block_size[id] - t, (int)dp_prev.size() - 1);
            if (lim >= 0) dp_cur[t] = prefix_best[lim] + profit[t];
        }
        dp_prev.swap(dp_cur);
    }

    ll best_profit = *max_element(dp_prev.begin(), dp_prev.end());
    return base - best_profit;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<int> r(n), b(m);
    for (int i = 0; i < n; ++i) scanf("%d", &r[i]);
    for (int i = 0; i < m; ++i) scanf("%d", &b[i]);
    printf("%lld\n", min_total_length(r, b));
    return 0;
}
