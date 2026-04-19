#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long xc, yc, xa, ya;
    cin >> n >> xc >> yc >> xa >> ya;

    vector<pair<long long, long long>> segs;
    segs.reserve(n);

    if (ya >= yc) {
        for (int i = 0; i < n; ++i) {
            long long x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
        }
        cout << fixed << setprecision(10) << 0.0 << '\n';
        return 0;
    }

    long long need_low = ya;
    long long need_high = yc;
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        long long l = max(need_low, y1);
        long long r = min(need_high, y2);
        if (l < r) {
            segs.push_back({l, r});
        }
    }

    sort(segs.begin(), segs.end());
    long long covered = 0;
    long long cur_l = 0, cur_r = 0;
    bool started = false;
    for (const auto& seg : segs) {
        if (!started) {
            cur_l = seg.first;
            cur_r = seg.second;
            started = true;
        } else if (seg.first <= cur_r) {
            cur_r = max(cur_r, seg.second);
        } else {
            covered += cur_r - cur_l;
            cur_l = seg.first;
            cur_r = seg.second;
        }
    }
    if (started) {
        covered += cur_r - cur_l;
    }

    double ans = double((need_high - need_low) - covered);
    cout << fixed << setprecision(10) << ans << '\n';
    return 0;
}
