#include <bits/stdc++.h>
using namespace std;

namespace {

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long long capped_choose(int n, int k, long long cap) {
    if (k < 0 || k > n) {
        return 0;
    }
    k = min(k, n - k);
    long long result = 1;
    for (int i = 1; i <= k; ++i) {
        long long num = n - k + i;
        long long g = gcd_ll(num, static_cast<long long>(i));
        num /= g;
        long long den = i / g;
        result /= gcd_ll(result, den);
        if (result > cap / num) {
            return cap;
        }
        result *= num;
    }
    return min(result, cap);
}

void generate_patterns(int k, int need_ones, int n, int pos, string& current,
                       vector<string>& patterns) {
    if (static_cast<int>(patterns.size()) == n) {
        return;
    }
    if (pos == k) {
        if (need_ones == 0) {
            patterns.push_back(current);
        }
        return;
    }

    int remaining = k - pos;
    if (need_ones > remaining) {
        return;
    }

    if (need_ones < remaining) {
        current[pos] = '0';
        generate_patterns(k, need_ones, n, pos + 1, current, patterns);
    }
    if (need_ones > 0) {
        current[pos] = '1';
        generate_patterns(k, need_ones - 1, n, pos + 1, current, patterns);
    }
}

void solve() {
    int n, w;
    cin >> n >> w;

    int best = -1;
    for (int k = 1; k <= w; ++k) {
        int ones = (k + 1) / 2;
        if (capped_choose(k - 1, ones, n) >= n) {
            best = k;
            break;
        }
    }

    if (best == -1) {
        cout << "infinity\n";
        return;
    }

    int ones = (best + 1) / 2;
    vector<string> patterns;
    string current(best, '0');
    generate_patterns(best, ones, n, 1, current, patterns);

    cout << best << '\n';
    for (int week = 0; week < w; ++week) {
        string schedule;
        schedule.reserve(n);
        for (int team = 0; team < n; ++team) {
            schedule.push_back(patterns[team][week % best] == '0' ? '1' : '2');
        }
        cout << schedule << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
