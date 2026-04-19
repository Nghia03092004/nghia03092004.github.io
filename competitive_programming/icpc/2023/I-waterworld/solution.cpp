#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n, m;
    cin >> n >> m;

    long double sum = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int x;
            cin >> x;
            sum += x;
        }
    }

    cout << fixed << setprecision(9) << sum / (n * m) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
