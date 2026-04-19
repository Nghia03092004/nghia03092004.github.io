#include <bits/stdc++.h>
using namespace std;

namespace {

long long distance_to_center(long long pos, long long size) {
    long long low = (size + 1) / 2;
    long long high = (size + 2) / 2;
    return min(llabs(pos - low), llabs(pos - high));
}

void solve() {
    long long r, c;
    cin >> r >> c;

    const long long q = c - 1;
    tuple<long long, long long, long long, long long, long long> best = {
        (long long)4e18, (long long)4e18, 1LL, 1LL, 1LL
    };

    for (long long p = 1; p <= c; ++p) {
        long long a = (p - 1) * r;
        long long row0 = -1;
        long long col = -1;

        if (p == 1) {
            row0 = 0;
            col = 1;
        } else if (p == c) {
            row0 = r - 1;
            col = c;
        } else {
            long long rem = a % q;
            if (rem == 0) {
                continue;
            }
            row0 = a / q;
            col = rem + 1;
        }

        long long lo = 0;
        long long hi = r - 1;
        long long row_steps = 0;
        while (lo != row0 || hi != row0) {
            lo = (a + lo) / c;
            hi = (a + hi) / c;
            ++row_steps;
        }

        long long row = row0 + 1;
        long long dist = distance_to_center(row, r) + distance_to_center(col, c);
        long long steps = row_steps + 1;
        auto candidate = make_tuple(dist, p, row, col, steps);
        if (candidate < best) {
            best = candidate;
        }
    }

    cout << get<1>(best) << ' '
         << get<2>(best) << ' '
         << get<3>(best) << ' '
         << get<4>(best) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
