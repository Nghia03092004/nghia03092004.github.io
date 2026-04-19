#include <bits/stdc++.h>
using namespace std;

int signed_shift(int x, int n) {
    return (2 * x <= n ? x : x - n);
}

int solve_orientation(const vector<int> &order) {
    int n = (int)order.size();
    int low = 1 - (n + 1) / 2;
    vector<char> seen(n, 0);

    for (int i = 0; i < n; ++i) {
        int delta = (i - (order[i] - 1)) % n;
        if (delta < 0) delta += n;
        int move = signed_shift(delta, n);
        seen[move - low] = 1;
    }

    int first = 0;
    while (first < n && !seen[first]) ++first;
    if (first == n) return 0;

    int best_gap = 0;
    int current_gap = 0;
    for (int step = 1; step <= n; ++step) {
        int idx = (first + step) % n;
        if (!seen[idx]) {
            ++current_gap;
        } else {
            best_gap = max(best_gap, current_gap);
            current_gap = 0;
        }
    }

    return (n - best_gap) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];

    vector<int> reversed(n);
    reversed[0] = p[0];
    for (int i = 1; i < n; ++i) reversed[i] = p[n - i];

    cout << min(solve_orientation(p), solve_orientation(reversed)) << '\n';
    return 0;
}
