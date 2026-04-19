#include <bits/stdc++.h>
using namespace std;

namespace {

long long floor_div(long long a, long long b) {
    if (a >= 0) {
        return a / b;
    }
    return -((-a + b - 1) / b);
}

void add_interval(long long A, long long B, int L, int R, vector<pair<int, int>>& events) {
    if (L > R) {
        return;
    }

    long long lo = L;
    long long hi = R;
    if (B == 0) {
        if (A >= 0) {
            return;
        }
    } else if (B > 0) {
        hi = min<long long>(hi, floor_div(-A - 1, B));
    } else {
        long long C = -B;
        lo = max<long long>(lo, floor_div(A, C) + 1);
    }

    if (lo > hi) {
        return;
    }
    events.push_back({static_cast<int>(lo), +1});
    events.push_back({static_cast<int>(hi + 1), -1});
}

void solve() {
    int p, h;
    cin >> p >> h;
    vector<vector<int>> scores(p, vector<int>(h));
    int max_score = 0;
    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < h; ++j) {
            cin >> scores[i][j];
            max_score = max(max_score, scores[i][j]);
        }
        sort(scores[i].begin(), scores[i].end());
    }

    vector<int> answer(p, p);
    for (int i = 0; i < p; ++i) {
        vector<pair<int, int>> events;
        events.reserve(4 * p * h);

        for (int j = 0; j < p; ++j) {
            if (i == j) {
                continue;
            }

            const vector<int>& a = scores[i];
            const vector<int>& b = scores[j];
            vector<int> breaks;
            breaks.reserve(2 * h);
            breaks.insert(breaks.end(), a.begin(), a.end());
            breaks.insert(breaks.end(), b.begin(), b.end());
            sort(breaks.begin(), breaks.end());
            breaks.erase(unique(breaks.begin(), breaks.end()), breaks.end());

            int pa = 0;
            int pb = 0;
            long long sum_a = 0;
            long long sum_b = 0;
            int current = 1;

            for (int s : breaks) {
                if (current <= s - 1) {
                    long long A = sum_a - sum_b;
                    long long B = static_cast<long long>(h - pa) - static_cast<long long>(h - pb);
                    add_interval(A, B, current, s - 1, events);
                }

                while (pa < h && a[pa] == s) {
                    sum_a += a[pa];
                    ++pa;
                }
                while (pb < h && b[pb] == s) {
                    sum_b += b[pb];
                    ++pb;
                }
                current = s;
            }

            if (current <= max_score) {
                long long A = sum_a - sum_b;
                long long B = static_cast<long long>(h - pa) - static_cast<long long>(h - pb);
                add_interval(A, B, current, max_score, events);
            }
        }

        sort(events.begin(), events.end());
        int active = 0;
        int best = 0;
        int idx = 0;
        while (idx < static_cast<int>(events.size())) {
            int x = events[idx].first;
            if (x > max_score) {
                break;
            }
            while (idx < static_cast<int>(events.size()) && events[idx].first == x) {
                active += events[idx].second;
                ++idx;
            }
            best = max(best, active);
        }

        answer[i] = p - best;
    }

    for (int x : answer) {
        cout << x << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
