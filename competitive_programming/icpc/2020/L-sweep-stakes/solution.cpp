#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr double ALIAS_EPS = 1e-11;

struct Query {
    vector<int> cells;
};

double bernstein_tail_bound(double variance, int distance) {
    if (distance <= 0) {
        return 1.0;
    }
    double denom = 2.0 * (variance + distance / 3.0);
    return 2.0 * exp(-(double)distance * distance / denom);
}

vector<double> subset_distribution(const vector<double>& probs) {
    vector<double> dp(probs.size() + 1, 0.0);
    dp[0] = 1.0;
    int used = 0;
    for (double p : probs) {
        for (int k = used; k >= 0; --k) {
            dp[k + 1] += dp[k] * p;
            dp[k] *= 1.0 - p;
        }
        ++used;
    }
    return dp;
}

double solve_tilt(const vector<double>& base, int target) {
    if (target == 0) {
        return 0.0;
    }
    int total = (int)base.size();
    if (target == total) {
        return numeric_limits<double>::infinity();
    }

    auto mean_at = [&](double z) {
        double sum = 0.0;
        for (double a : base) {
            sum += a * z / (1.0 - a + a * z);
        }
        return sum;
    };

    double lo = 0.0;
    double hi = 1.0;
    while (mean_at(hi) < target) {
        hi *= 2.0;
    }
    for (int it = 0; it < 100; ++it) {
        double mid = (lo + hi) * 0.5;
        if (mean_at(mid) < target) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return (lo + hi) * 0.5;
}

vector<double> build_mod_distribution(const vector<double>& probs, int mod) {
    vector<double> cur(mod, 0.0), nxt(mod, 0.0);
    cur[0] = 1.0;
    for (double p : probs) {
        double q = 1.0 - p;
        nxt[0] = cur[0] * q + cur[mod - 1] * p;
        for (int r = 1; r < mod; ++r) {
            nxt[r] = cur[r] * q + cur[r - 1] * p;
        }
        cur.swap(nxt);
    }
    return cur;
}

void remove_cell_forward(const vector<double>& src, double p, vector<double>& dst) {
    int mod = (int)src.size();
    double a = 1.0 - p;
    double b = p;

    double u = 0.0;
    double v = 1.0;
    for (int r = 1; r < mod; ++r) {
        u = (src[r] - b * u) / a;
        v = (-b * v) / a;
    }
    double x = (src[0] - b * u) / (a + b * v);

    dst[0] = x;
    for (int r = 1; r < mod; ++r) {
        dst[r] = (src[r] - b * dst[r - 1]) / a;
    }
}

void remove_cell_backward(const vector<double>& src, double p, vector<double>& dst) {
    int mod = (int)src.size();
    double a = 1.0 - p;
    double b = p;

    double u = 0.0;
    double v = 1.0;
    for (int r = mod - 2; r >= 0; --r) {
        u = (src[r + 1] - a * u) / b;
        v = (-a * v) / b;
    }
    double x = (src[0] - a * u) / (b + a * v);

    dst[mod - 1] = x;
    for (int r = mod - 2; r >= 0; --r) {
        dst[r] = (src[r + 1] - a * dst[r + 1]) / b;
    }
}

void remove_cell(const vector<double>& src, double p, vector<double>& dst) {
    if (p < 1e-15) {
        dst = src;
        return;
    }
    if (1.0 - p < 1e-15) {
        int mod = (int)src.size();
        for (int r = 0; r < mod; ++r) {
            dst[r] = src[(r + 1) % mod];
        }
        return;
    }
    if (p <= 0.5) {
        remove_cell_forward(src, p, dst);
    } else {
        remove_cell_backward(src, p, dst);
    }
}

void solve() {
    int m = 0, n = 0, t = 0, q = 0;
    cin >> m >> n >> t >> q;

    vector<double> row(m), col(n);
    for (double& x : row) {
        cin >> x;
    }
    for (double& x : col) {
        cin >> x;
    }

    vector<Query> queries(q);
    int max_s = 0;
    for (int qi = 0; qi < q; ++qi) {
        int s = 0;
        cin >> s;
        max_s = max(max_s, s);
        queries[qi].cells.resize(s);
        for (int i = 0; i < s; ++i) {
            int r = 0, c = 0;
            cin >> r >> c;
            --r;
            --c;
            queries[qi].cells[i] = r * n + c;
        }
    }

    int total_cells = m * n;
    if (t == 0) {
        for (const Query& query : queries) {
            cout << fixed << setprecision(9) << 1.0;
            for (int i = 0; i < (int)query.cells.size(); ++i) {
                cout << ' ' << 0.0;
            }
            cout << '\n';
        }
        return;
    }
    if (t == total_cells) {
        for (const Query& query : queries) {
            int s = (int)query.cells.size();
            for (int i = 0; i < s; ++i) {
                cout << fixed << setprecision(9) << 0.0 << ' ';
            }
            cout << fixed << setprecision(9) << 1.0 << '\n';
        }
        return;
    }

    vector<double> base(total_cells);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            base[i * n + j] = row[i] + col[j];
        }
    }

    double z = solve_tilt(base, t);
    vector<double> tilted(total_cells);
    double variance = 0.0;
    for (int idx = 0; idx < total_cells; ++idx) {
        double a = base[idx];
        double p = a * z / (1.0 - a + a * z);
        tilted[idx] = p;
        variance += p * (1.0 - p);
    }

    int mod = max_s + 1;
    if ((mod & 1) == 0) {
        ++mod;
    }
    while (bernstein_tail_bound(variance, mod - max_s) > ALIAS_EPS) {
        mod += 2;
    }

    vector<double> total_mod = build_mod_distribution(tilted, mod);
    double denom = total_mod[t % mod];
    if (denom < 1e-300) {
        denom = 1e-300;
    }

    vector<double> cur = total_mod;
    vector<double> tmp(mod);

    for (const Query& query : queries) {
        vector<double> sel_prob;
        sel_prob.reserve(query.cells.size());
        for (int cell : query.cells) {
            sel_prob.push_back(tilted[cell]);
        }

        vector<double> inside = subset_distribution(sel_prob);
        cur = total_mod;
        for (double p : sel_prob) {
            remove_cell(cur, p, tmp);
            cur.swap(tmp);
        }

        vector<double> answer(query.cells.size() + 1, 0.0);
        double sum_ans = 0.0;
        for (int r = 0; r <= (int)query.cells.size(); ++r) {
            int residue = (t - r) % mod;
            if (residue < 0) {
                residue += mod;
            }
            double val = inside[r] * cur[residue] / denom;
            if (val < 0.0 && val > -1e-12) {
                val = 0.0;
            }
            answer[r] = val;
            sum_ans += val;
        }

        if (sum_ans > 0.0) {
            for (double& x : answer) {
                x = max(0.0, x / sum_ans);
            }
        }

        for (int i = 0; i < (int)answer.size(); ++i) {
            if (i) {
                cout << ' ';
            }
            cout << fixed << setprecision(9) << answer[i];
        }
        cout << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
