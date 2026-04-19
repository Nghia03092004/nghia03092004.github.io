#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<long long> mn, lazy;

    SegTree(int n) : n(n), mn(4 * n, 0), lazy(4 * n, 0) {}

    void push(int v) {
        if (lazy[v] != 0) {
            for (int c : {2*v, 2*v+1}) {
                mn[c] += lazy[v];
                lazy[c] += lazy[v];
            }
            lazy[v] = 0;
        }
    }

    void update(int v, int l, int r, int ql, int qr, long long val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            mn[v] += val;
            lazy[v] += val;
            return;
        }
        push(v);
        int mid = (l + r) / 2;
        update(2*v, l, mid, ql, qr, val);
        update(2*v+1, mid+1, r, ql, qr, val);
        mn[v] = min(mn[2*v], mn[2*v+1]);
    }

    long long query() { return mn[1]; }

    void update(int ql, int qr, long long val) {
        if (ql > qr) return;
        update(1, 1, n, ql, qr, val);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, P;
    long long B;
    cin >> R >> C >> P >> B;

    vector<int> x1(P), y1(P), x2(P), y2(P);
    vector<long long> cost(P);
    for (int i = 0; i < P; i++) {
        cin >> x1[i] >> y1[i] >> x2[i] >> y2[i] >> cost[i];
    }

    // Binary search on side length s
    int lo = 1, hi = min(R, C), ans = 0;

    while (lo <= hi) {
        int s = (lo + hi) / 2;

        // Check feasibility: can we place an s x s square with total cost <= B?
        int maxR = R - s + 1; // top-left row range: 1..maxR
        int maxC = C - s + 1; // top-left col range: 1..maxC

        if (maxR < 1 || maxC < 1) {
            hi = s - 1;
            continue;
        }

        // Create events
        // events[r] = list of (col_start, col_end, cost_delta)
        vector<vector<tuple<int,int,long long>>> events(maxR + 2);

        for (int i = 0; i < P; i++) {
            int rs = max(1, x1[i] - s + 1);
            int re = min(maxR, x2[i]);
            if (rs > re) continue;

            int cs = max(1, y1[i] - s + 1);
            int ce = min(maxC, y2[i]);
            if (cs > ce) continue;

            events[rs].push_back({cs, ce, cost[i]});
            if (re + 1 <= maxR)
                events[re + 1].push_back({cs, ce, -cost[i]});
        }

        // Sweep
        SegTree seg(maxC);
        bool feasible = false;

        for (int r = 1; r <= maxR; r++) {
            for (auto& [cs, ce, delta] : events[r]) {
                seg.update(cs, ce, delta);
            }
            if (seg.query() <= B) {
                feasible = true;
                break;
            }
        }

        if (feasible) {
            ans = s;
            lo = s + 1;
        } else {
            hi = s - 1;
        }
    }

    cout << ans << "\n";
    return 0;
}
