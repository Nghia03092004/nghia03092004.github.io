#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Segment tree on values (by coordinate-compressed index)
// Supports: insert, erase, query top-k sum
struct SegTree {
    int n;
    vector<int> cnt;
    vector<ll> sum;

    void init(int _n) {
        n = _n;
        cnt.assign(4 * n, 0);
        sum.assign(4 * n, 0);
    }

    void update(int node, int l, int r, int pos, int val, ll w) {
        if (l == r) {
            cnt[node] += val;
            sum[node] += w;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2*node, l, mid, pos, val, w);
        else update(2*node+1, mid+1, r, pos, val, w);
        cnt[node] = cnt[2*node] + cnt[2*node+1];
        sum[node] = sum[2*node] + sum[2*node+1];
    }

    // Sum of top-k elements
    ll query(int node, int l, int r, int k) {
        if (k <= 0) return 0;
        if (k >= cnt[node]) return sum[node];
        if (l == r) return (ll)k * (sum[node] / cnt[node]);
        int mid = (l + r) / 2;
        if (cnt[2*node+1] >= k)
            return query(2*node+1, mid+1, r, k);
        else
            return sum[2*node+1] + query(2*node, l, mid, k - cnt[2*node+1]);
    }

    void add(int pos, ll w) { update(1, 0, n-1, pos, 1, w); }
    void rem(int pos, ll w) { update(1, 0, n-1, pos, -1, -w); }
    ll topk(int k) { return query(1, 0, n-1, k); }
};

int N, S, D;
vector<int> a;
vector<int> sorted_vals;
SegTree seg;
ll best_ans;

int compress(int v) {
    return lower_bound(sorted_vals.begin(), sorted_vals.end(), v) - sorted_vals.begin();
}

int cur_l, cur_r; // current window in seg tree

void addCity(int i) { seg.add(compress(a[i]), a[i]); }
void remCity(int i) { seg.rem(compress(a[i]), a[i]); }

void expand_to(int l, int r) {
    while (cur_r < r) { cur_r++; addCity(cur_r); }
    while (cur_l > l) { cur_l--; addCity(cur_l); }
    while (cur_r > r) { remCity(cur_r); cur_r--; }
    while (cur_l < l) { remCity(cur_l); cur_l++; }
}

// left-first: cost = 2*(S-l) + (r-S), visit = D - cost
// right-first: cost = (S-l) + 2*(r-S), visit = D - cost
void solve(int lo_l, int hi_l, int lo_r, int hi_r, bool leftFirst) {
    if (lo_l > hi_l) return;
    int mid_l = (lo_l + hi_l) / 2;
    int best_r = lo_r;
    ll best_val = -1;

    int rmin = max(lo_r, S);
    int rmax = min(hi_r, N - 1);

    for (int r = rmin; r <= rmax; r++) {
        int l = mid_l;
        int cost;
        if (leftFirst) cost = 2 * (S - l) + (r - S);
        else cost = (S - l) + 2 * (r - S);
        int visit = D - cost;
        if (visit <= 0) continue;
        visit = min(visit, r - l + 1);

        expand_to(l, r);
        ll val = seg.topk(visit);
        if (val > best_val) {
            best_val = val;
            best_r = r;
        }
    }
    best_ans = max(best_ans, best_val);
    solve(lo_l, mid_l - 1, lo_r, best_r, leftFirst);
    solve(mid_l + 1, hi_l, best_r, hi_r, leftFirst);
}

long long findMaxAttraction(int n, int start, int d, int attraction[]) {
    N = n; S = start; D = d;
    a.assign(attraction, attraction + n);
    sorted_vals = a;
    sort(sorted_vals.begin(), sorted_vals.end());
    sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());

    best_ans = 0;

    // Left-first: l in [0, S], r in [S, N-1]
    seg.init(sorted_vals.size());
    cur_l = S; cur_r = S - 1;
    solve(0, S, S, N - 1, true);

    // Right-first: l in [0, S], r in [S, N-1]
    seg.init(sorted_vals.size());
    cur_l = S; cur_r = S - 1;
    solve(0, S, S, N - 1, false);

    return best_ans;
}

int main() {
    int n, s, d;
    scanf("%d %d %d", &n, &s, &d);
    int attr[n];
    for (int i = 0; i < n; i++) scanf("%d", &attr[i]);
    printf("%lld\n", findMaxAttraction(n, s, d, attr));
    return 0;
}
