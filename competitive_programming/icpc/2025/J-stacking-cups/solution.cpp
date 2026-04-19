#include <bits/stdc++.h>
using namespace std;

long long isqrt_ll(long long x) {
    long long r = sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) {
        ++r;
    }
    while (r * r > x) {
        --r;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long h;
    cin >> n >> h;

    long long min_h = 2LL * n - 1;
    long long max_h = 1LL * n * n;
    if (h < min_h || h > max_h) {
        cout << "impossible\n";
        return 0;
    }

    for (int b = 1; b <= n; ++b) {
        long long t = h - (n - b);
        if (t < 0) {
            continue;
        }

        long long a = isqrt_ll(t);
        if ((a & 1LL) != (t & 1LL)) {
            --a;
        }
        if (a < 0) {
            continue;
        }

        long long low = a * a;
        long long high = a * (2LL * b - a);
        if (!(low <= t && t <= high)) {
            continue;
        }

        long long need_sum = (t + a) / 2;
        vector<int> inc;
        inc.reserve((size_t)a);
        for (int i = 1; i <= a; ++i) {
            inc.push_back(i);
        }

        long long cur_sum = a * (a + 1) / 2;
        for (int i = (int)a - 1; i >= 0; --i) {
            int max_here = b - ((int)a - 1 - i);
            long long add = min<long long>(max_here - inc[i], need_sum - cur_sum);
            inc[i] += (int)add;
            cur_sum += add;
        }

        vector<int> order;
        order.reserve(n);
        for (int x = n; x > b; --x) {
            order.push_back(x);
        }
        vector<char> used(b + 1, 0);
        for (int x : inc) {
            used[x] = 1;
            order.push_back(x);
        }
        for (int x = b; x >= 1; --x) {
            if (!used[x]) {
                order.push_back(x);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (i) {
                cout << ' ';
            }
            cout << 2LL * order[i] - 1;
        }
        cout << '\n';
        return 0;
    }

    cout << "impossible\n";
    return 0;
}
