// IOI 2002 - Batch Scheduling
// N jobs processed in order, grouped into consecutive batches with setup time S.
// Minimize total weighted completion time: sum of f_i * C_i.
// DP: dp[j] = min cost for jobs 1..j where j ends a batch.
// dp[j] = min over k { dp[k] + (S + T[j] - T[k]) * (F[N] - F[k]) }
// Optimized with Convex Hull Trick for O(N) total time.
//
// CHT formulation:
//   slope_k   = F[N] - F[k]        (decreasing, since F[k] increases)
//   intercept = dp[k] + (S - T[k]) * (F[N] - F[k])
//   query x   = T[j]               (increasing)

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long S;
    cin >> N >> S;

    vector<long long> t(N + 1), f(N + 1);
    vector<long long> T(N + 1, 0), F(N + 1, 0); // prefix sums

    for (int i = 1; i <= N; i++) {
        cin >> t[i] >> f[i];
        T[i] = T[i - 1] + t[i];
        F[i] = F[i - 1] + f[i];
    }

    vector<long long> dp(N + 1, 0);

    // Convex Hull Trick: minimize y = m*x + b with decreasing slopes
    // and increasing query points => use monotone deque.
    struct Line {
        long long m, b;
        long long eval(long long x) const { return m * x + b; }
    };

    auto bad = [](const Line& l1, const Line& l2, const Line& l3) -> bool {
        // l2 is unnecessary if intersection of l1,l3 is to the left of l1,l2
        return (__int128)(l3.b - l1.b) * (l1.m - l2.m) <=
               (__int128)(l2.b - l1.b) * (l1.m - l3.m);
    };

    deque<Line> hull;

    auto addLine = [&](long long slope, long long intercept) {
        Line newLine = {slope, intercept};
        while (hull.size() >= 2 &&
               bad(hull[hull.size() - 2], hull[hull.size() - 1], newLine)) {
            hull.pop_back();
        }
        hull.push_back(newLine);
    };

    auto query = [&](long long x) -> long long {
        while (hull.size() >= 2 && hull[0].eval(x) >= hull[1].eval(x)) {
            hull.pop_front();
        }
        return hull[0].eval(x);
    };

    // Add line for k = 0
    // slope = F[N] - F[0] = F[N], intercept = dp[0] + (S - T[0])*(F[N] - F[0]) = S*F[N]
    addLine(F[N], S * F[N]);

    for (int j = 1; j <= N; j++) {
        dp[j] = query(T[j]);

        // Add line for k = j
        long long slope = F[N] - F[j];
        long long intercept = dp[j] + (S - T[j]) * (F[N] - F[j]);
        addLine(slope, intercept);
    }

    cout << dp[N] << "\n";

    return 0;
}
