#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n;
    cin >> n;

    vector<array<long long, 3>> phones(n);
    array<long long, 8> best_sum;
    best_sum.fill(0);

    for (int i = 0; i < n; ++i) {
        cin >> phones[i][0] >> phones[i][1] >> phones[i][2];
        for (int mask = 1; mask < 8; ++mask) {
            long long sum = 0;
            for (int bit = 0; bit < 3; ++bit) {
                if (mask & (1 << bit)) {
                    sum += phones[i][bit];
                }
            }
            best_sum[mask] = max(best_sum[mask], sum);
        }
    }

    pair<long long, int> answer = {LLONG_MAX, -1};
    for (int i = 0; i < n; ++i) {
        long long cost = 0;
        for (int mask = 1; mask < 8; ++mask) {
            long long sum = 0;
            for (int bit = 0; bit < 3; ++bit) {
                if (mask & (1 << bit)) {
                    sum += phones[i][bit];
                }
            }
            cost = max(cost, best_sum[mask] - sum);
        }
        answer = min(answer, {cost, i + 1});
    }

    cout << answer.first << ' ' << answer.second << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
