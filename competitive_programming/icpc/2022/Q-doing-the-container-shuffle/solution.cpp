#include <bits/stdc++.h>
using namespace std;

namespace {

struct Fenwick {
    int n = 0;
    vector<int> bit;

    explicit Fenwick(int size) : n(size), bit(size + 1, 0) {}

    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    int sum_prefix(int idx) const {
        int result = 0;
        for (; idx > 0; idx -= idx & -idx) {
            result += bit[idx];
        }
        return result;
    }
};

void solve() {
    int n;
    cin >> n;

    vector<int> order(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> order[i];
    }

    Fenwick fenwick(n);
    long double answer = static_cast<long double>(n);

    for (int i = n; i >= 1; --i) {
        int threshold = (i == 1 ? order[1] : min(order[i - 1], order[i]));
        int greater = fenwick.sum_prefix(n) - fenwick.sum_prefix(threshold);
        answer += 0.5L * greater;
        fenwick.add(order[i], 1);
    }

    cout << fixed << setprecision(3) << static_cast<double>(answer) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
