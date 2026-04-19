#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n, k;
    cin >> n >> k;

    unordered_map<long long, pair<int, int>> info;
    info.reserve(static_cast<size_t>(n * k * 2));

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < k; ++j) {
            long long x;
            cin >> x;
            auto& entry = info[x];
            ++entry.first;
            entry.second = i;
        }
    }

    vector<long long> owned(n + 1, 0);
    for (const auto& it : info) {
        const int total = it.second.first;
        const int owner = it.second.second;
        owned[owner] += total;
    }

    const double denom = static_cast<double>(n) * static_cast<double>(k);
    cout << fixed << setprecision(9);
    for (int i = 1; i <= n; ++i) {
        cout << owned[i] / denom << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
