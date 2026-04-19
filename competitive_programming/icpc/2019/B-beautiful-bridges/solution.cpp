#include <bits/stdc++.h>
using namespace std;

namespace {

using int64 = long long;
using real = double;

const real EPS = 1e-10;
const int64 INF64 = (1LL << 62);

struct Limit {
    real value;
    bool inclusive;
};

real threshold_on_circle(real t, real slope, real p) {
    real clearance = p - slope * t;
    return (t * t + clearance * clearance) / (2.0 * t);
}

void relax_limit(Limit& best, real cand_value, bool cand_inclusive) {
    if (cand_value + EPS < best.value) {
        best.value = cand_value;
        best.inclusive = cand_inclusive;
        return;
    }
    if (fabs(cand_value - best.value) <= EPS) {
        best.inclusive = best.inclusive && cand_inclusive;
    }
}

bool fits_limit(const Limit& limit, real radius) {
    if (radius + EPS < limit.value) {
        return true;
    }
    if (radius > limit.value + EPS) {
        return false;
    }
    return limit.inclusive;
}

vector<Limit> compute_half_limits(const vector<int>& x, const vector<int>& y, int h) {
    int n = (int)x.size();
    vector<real> slope(n - 1), inv_norm(n - 1);
    for (int i = 0; i + 1 < n; ++i) {
        slope[i] = (real)(y[i + 1] - y[i]) / (real)(x[i + 1] - x[i]);
        inv_norm[i] = 1.0 / sqrt(1.0 + slope[i] * slope[i]);
    }

    vector<Limit> limits(n, {numeric_limits<real>::infinity(), true});
    for (int i = 0; i < n; ++i) {
        Limit best = {numeric_limits<real>::infinity(), true};
        for (int seg = i; seg + 1 < n; ++seg) {
            real u = (real)x[seg] - (real)x[i];
            real v = (real)x[seg + 1] - (real)x[i];
            real s = slope[seg];
            real p = (real)(h - y[seg]) + s * u;

            real phi_hi = v;
            if (s > -1.0) {
                phi_hi = min(phi_hi, p / (s + 1.0));
            }
            if (phi_hi + EPS >= u) {
                real lo = max(u, 0.0);
                real hi = phi_hi;
                if (hi + EPS >= lo) {
                    real t = p * inv_norm[seg];
                    if (t < lo) {
                        t = lo;
                    }
                    if (t > hi) {
                        t = hi;
                    }
                    relax_limit(best, threshold_on_circle(t, s, p), true);
                }
            }

            if (s > -1.0) {
                real cut = p / (s + 1.0);
                if (cut < v - EPS) {
                    if (cut < u - EPS) {
                        relax_limit(best, u, false);
                    } else {
                        relax_limit(best, max(cut, u), true);
                    }
                }
            }
        }
        limits[i] = best;
    }
    return limits;
}

void solve() {
    int n, h, alpha, beta;
    cin >> n >> h >> alpha >> beta;
    vector<int> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    vector<Limit> left_limits = compute_half_limits(x, y, h);

    vector<int> rev_x(n), rev_y(n);
    for (int i = 0; i < n; ++i) {
        rev_x[i] = -x[n - 1 - i];
        rev_y[i] = y[n - 1 - i];
    }
    vector<Limit> rev_limits = compute_half_limits(rev_x, rev_y, h);
    vector<Limit> right_limits(n);
    for (int i = 0; i < n; ++i) {
        right_limits[i] = rev_limits[n - 1 - i];
    }

    vector<int64> dp(n, INF64);
    dp[0] = (int64)alpha * (h - y[0]);

    for (int j = 1; j < n; ++j) {
        int64 pillar_cost = (int64)alpha * (h - y[j]);
        for (int i = 0; i < j; ++i) {
            if (dp[i] == INF64) {
                continue;
            }
            int64 dx = (int64)x[j] - (int64)x[i];
            real radius = (real)dx / 2.0;
            if (!fits_limit(left_limits[i], radius) || !fits_limit(right_limits[j], radius)) {
                continue;
            }
            int64 candidate = dp[i] + pillar_cost + (int64)beta * dx * dx;
            if (candidate < dp[j]) {
                dp[j] = candidate;
            }
        }
    }

    if (dp[n - 1] == INF64) {
        cout << "impossible\n";
    } else {
        cout << dp[n - 1] << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
