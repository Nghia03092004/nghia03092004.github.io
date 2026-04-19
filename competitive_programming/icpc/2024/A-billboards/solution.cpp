#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr long double BASE_EPS = 1e-15L;

long double clamp_value(long double x, long double low, long double high) {
    return min(high, max(low, x));
}

struct Sponsor {
    vector<long double> x;
    vector<long double> y;
    vector<long double> pref;
    long double total = 0;
    long double target = 0;

    long double area_eps() const {
        return BASE_EPS * max<long double>(1.0L, total);
    }

    long double prefix_at(long double pos) const {
        if (pos <= x.front()) {
            return 0;
        }
        if (pos >= x.back()) {
            return total;
        }
        int seg = int(upper_bound(x.begin(), x.end(), pos) - x.begin()) - 1;
        long double dx = pos - x[seg];
        long double len = x[seg + 1] - x[seg];
        long double slope = (y[seg + 1] - y[seg]) / len;
        return pref[seg] + y[seg] * dx + slope * dx * dx / 2.0L;
    }

    long double first_cut_after(long double start) const {
        long double goal = prefix_at(start) + target;
        long double eps = area_eps();
        if (goal >= total - eps) {
            return x.back();
        }

        int idx = int(lower_bound(pref.begin(), pref.end(), goal - eps) - pref.begin());
        if (idx >= int(pref.size())) {
            return x.back();
        }
        if (fabsl(pref[idx] - goal) <= eps) {
            return x[idx];
        }

        int seg = idx - 1;
        long double need = goal - pref[seg];
        long double len = x[seg + 1] - x[seg];
        long double base = y[seg];
        long double slope = (y[seg + 1] - y[seg]) / len;
        long double dx = 0;

        if (fabsl(slope) <= 1e-18L) {
            if (base <= 1e-18L) {
                return x[seg + 1];
            }
            dx = need / base;
        } else {
            long double disc = base * base + 2.0L * slope * need;
            if (disc < 0 && disc > -eps) {
                disc = 0;
            }
            disc = max<long double>(0.0L, disc);
            dx = (-base + sqrtl(disc)) / slope;
        }

        dx = clamp_value(dx, 0.0L, len);
        return x[seg] + dx;
    }

    long double value_on(long double left, long double right) const {
        return prefix_at(right) - prefix_at(left);
    }
};

void solve() {
    int n;
    long double l;
    cin >> n >> l;

    vector<Sponsor> sponsors(n);
    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        sponsors[i].x.resize(m);
        sponsors[i].y.resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> sponsors[i].x[j] >> sponsors[i].y[j];
        }

        sponsors[i].pref.assign(m, 0);
        for (int j = 0; j + 1 < m; ++j) {
            long double dx = sponsors[i].x[j + 1] - sponsors[i].x[j];
            long double avg = (sponsors[i].y[j] + sponsors[i].y[j + 1]) / 2.0L;
            sponsors[i].pref[j + 1] = sponsors[i].pref[j] + dx * avg;
        }
        sponsors[i].total = sponsors[i].pref.back();
        sponsors[i].target = sponsors[i].total / n;
    }

    vector<char> alive(n, true);
    vector<pair<long double, int>> answer;
    answer.reserve(n);

    long double left = 0;
    for (int step = 0; step + 1 < n; ++step) {
        long double best_cut = l + 1;
        int best_id = -1;
        for (int i = 0; i < n; ++i) {
            if (!alive[i]) {
                continue;
            }
            long double cut = sponsors[i].first_cut_after(left);
            if (cut < best_cut) {
                best_cut = cut;
                best_id = i;
            }
        }

        if (best_id == -1 || best_cut > l + 1e-12L) {
            cout << "impossible\n";
            return;
        }

        alive[best_id] = false;
        answer.push_back({best_cut, best_id + 1});
        left = best_cut;
    }

    int last_id = -1;
    for (int i = 0; i < n; ++i) {
        if (alive[i]) {
            last_id = i;
            break;
        }
    }
    if (last_id == -1) {
        cout << "impossible\n";
        return;
    }
    answer.push_back({l, last_id + 1});

    cout << fixed << setprecision(15);
    for (const auto& item : answer) {
        long double cut = item.first;
        int id = item.second;
        if (fabsl(cut) < 5e-16L) {
            cut = 0;
        }
        if (fabsl(cut - l) <= 5e-13L * max<long double>(1.0L, l)) {
            cut = l;
        }
        cout << cut << ' ' << id << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
