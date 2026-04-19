#include <bits/stdc++.h>
using namespace std;

namespace {

void add_linear(vector<long long>& diff_a,
                vector<long long>& diff_b,
                int left,
                int right,
                long long a,
                long long b) {
    if (left > right) {
        return;
    }
    diff_a[left] += a;
    diff_a[right + 1] -= a;
    diff_b[left] += b;
    diff_b[right + 1] -= b;
}

void solve() {
    int n, k;
    cin >> n >> k;

    vector<long long> diff_a(n + 2, 0);
    vector<long long> diff_b(n + 2, 0);

    for (int q = 0; q < k; ++q) {
        char type;
        int x1, x2;
        cin >> type >> x1 >> x2;

        if (type == 'R') {
            add_linear(diff_a, diff_b, x1, x2, 0, 1);
        } else if (type == 'D') {
            add_linear(diff_a, diff_b, x1, x2, 0, -1);
        } else {
            long long sign = (type == 'H' ? 1 : -1);
            int mid = (x1 + x2) / 2;

            add_linear(diff_a, diff_b, x1, mid, sign, sign * (1LL - x1));
            add_linear(diff_a, diff_b, mid + 1, x2, -sign, sign * (x2 + 1LL));
        }
    }

    long long current_a = 0;
    long long current_b = 0;
    for (int i = 1; i <= n; ++i) {
        current_a += diff_a[i];
        current_b += diff_b[i];
        cout << current_a * i + current_b << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
