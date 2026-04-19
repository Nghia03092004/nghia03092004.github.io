#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n;
    cin >> n;

    vector<long long> b(n), e(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i] >> e[i];
    }

    if (b[0] == 0) {
        cout << "impossible\n";
        return;
    }

    vector<pair<long long, long long>> chosen;
    long long reach = 0;
    for (int i = 0; i < n; ++i) {
        long long left = (i == 0 ? 0 : e[i - 1]);
        long long right = b[i];
        if (right <= left) {
            continue;
        }
        if (left > reach) {
            break;
        }
        long long extension = 3 * right - 2 * left;
        if (extension > reach) {
            chosen.push_back({left, right});
            reach = extension;
        }
    }

    if (chosen.empty() || reach < e.back()) {
        cout << "impossible\n";
        return;
    }

    for (int i = 0; i + 1 < static_cast<int>(chosen.size()); ++i) {
        long long limit = (chosen[i].first + chosen[i + 1].first) / 2;
        chosen[i].second = min(chosen[i].second, limit);
    }

    for (int i = 0; i < static_cast<int>(chosen.size()); ++i) {
        long long s = chosen[i].first;
        long long t = chosen[i].second;
        if (t <= s) {
            cout << "impossible\n";
            return;
        }
        if (i + 1 < static_cast<int>(chosen.size())) {
            long long next_start = chosen[i + 1].first;
            if (next_start < 2 * t - s || 3 * t - 2 * s < next_start) {
                cout << "impossible\n";
                return;
            }
        } else if (3 * t - 2 * s < e.back()) {
            cout << "impossible\n";
            return;
        }
    }

    cout << chosen.size() << '\n';
    for (const auto& interval : chosen) {
        cout << interval.first << ' ' << interval.second << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
