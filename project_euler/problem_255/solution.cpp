#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 255: Rounded Square Roots
 *
 * For n from 10^13 to 10^14-1 (14 digits, d=14, even): x0 = 7*10^6
 * Iteration: x_{k+1} = floor((x_k + ceil(n/x_k)) / 2)
 * Stop when x_{k+1} == x_k.
 *
 * Interval-based approach: track (x, n_lo, n_hi) groups and split by
 * ceil(n/x) values at each step. For fixed x, x_next = floor((x+q)/2)
 * where q = ceil(n/x). Each x_next value corresponds to 2 consecutive q values.
 *
 * Answer: 4.4474011180
 */

typedef long long ll;

ll ceildiv(ll a, ll b) {
    return (a + b - 1) / b;
}

int main(){
    const ll N_LO = 10000000000000LL;
    const ll N_HI = 99999999999999LL;
    const ll TOTAL = N_HI - N_LO + 1;
    const int x0 = 7000000;

    struct Interval {
        int x;
        ll nlo, nhi;
    };

    vector<Interval> current;
    current.push_back({x0, N_LO, N_HI});

    long double total_iterations = 0;

    while (!current.empty()) {
        // Process one iteration
        map<int, vector<pair<ll,ll>>> next_map;
        ll converged_count = 0;
        // We need iteration number for weighting. Track separately.
        // Actually we increment a counter.
        static int iter = 0;
        iter++;

        for (auto& [x, nlo, nhi] : current) {
            ll q_lo = ceildiv(nlo, (ll)x);
            ll q_hi = ceildiv(nhi, (ll)x);
            ll xn_lo = ((ll)x + q_lo) / 2;
            ll xn_hi = ((ll)x + q_hi) / 2;

            for (ll xn = xn_lo; xn <= xn_hi; xn++) {
                ll q1 = max(2LL * xn - x, q_lo);
                ll q2 = min(2LL * xn + 1 - x, q_hi);
                if (q1 > q2) continue;

                ll n1 = max((q1 - 1) * x + 1, nlo);
                ll n2 = min(q2 * x, nhi);
                if (n1 > n2) continue;

                if (xn == x) {
                    converged_count += n2 - n1 + 1;
                } else {
                    next_map[(int)xn].push_back({n1, n2});
                }
            }
        }

        total_iterations += (long double)converged_count * iter;

        current.clear();
        for (auto& [xn, intervals] : next_map) {
            sort(intervals.begin(), intervals.end());
            ll lo = intervals[0].first, hi = intervals[0].second;
            for (size_t i = 1; i < intervals.size(); i++) {
                if (intervals[i].first <= hi + 1) {
                    hi = max(hi, intervals[i].second);
                } else {
                    current.push_back({xn, lo, hi});
                    lo = intervals[i].first;
                    hi = intervals[i].second;
                }
            }
            current.push_back({xn, lo, hi});
        }
    }

    long double avg = total_iterations / (long double)TOTAL;
    printf("%.10Lf\n", avg);
    return 0;
}
