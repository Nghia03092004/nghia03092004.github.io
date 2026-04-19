#include <bits/stdc++.h>
using namespace std;

namespace {

struct Pipe {
    vector<pair<int, double>> station_edges;
    vector<double> reservoir_edges;
};

int s, r, d;
vector<vector<Pipe>> pipes;

double value_for_weights(const vector<double>& w) {
    vector<double> best(s + 1, 0.0);
    for (int u = s; u >= 1; --u) {
        double cur = -1e100;
        for (const Pipe& pipe : pipes[u]) {
            double cand = 0.0;
            for (int i = 0; i < r; ++i) {
                cand += w[i] * pipe.reservoir_edges[i];
            }
            for (const auto& edge : pipe.station_edges) {
                cand += edge.second * best[edge.first];
            }
            cur = max(cur, cand);
        }
        best[u] = cur;
    }
    return best[1];
}

double golden_min(const function<double(double)>& f, double lo, double hi, int iters) {
    const double PHI = (sqrt(5.0) - 1.0) / 2.0;
    double x1 = hi - PHI * (hi - lo);
    double x2 = lo + PHI * (hi - lo);
    double y1 = f(x1);
    double y2 = f(x2);
    for (int it = 0; it < iters; ++it) {
        if (y1 > y2) {
            lo = x1;
            x1 = x2;
            y1 = y2;
            x2 = lo + PHI * (hi - lo);
            y2 = f(x2);
        } else {
            hi = x2;
            x2 = x1;
            y2 = y1;
            x1 = hi - PHI * (hi - lo);
            y1 = f(x1);
        }
    }
    return min(y1, y2);
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> s >> r >> d;
    pipes.assign(s + 1, {});
    for (int i = 0; i < d; ++i) {
        int at, cnt;
        cin >> at >> cnt;
        Pipe pipe;
        pipe.reservoir_edges.assign(r, 0.0);
        for (int j = 0; j < cnt; ++j) {
            int to, p;
            cin >> to >> p;
            double frac = p / 100.0;
            if (to <= s) {
                pipe.station_edges.push_back({to, frac});
            } else {
                pipe.reservoir_edges[to - s - 1] += frac;
            }
        }
        pipes[at].push_back(move(pipe));
    }

    cout << fixed << setprecision(10);
    if (r == 1) {
        cout << 100.0 * value_for_weights(vector<double>(1, 1.0)) << '\n';
        return 0;
    }

    if (r == 2) {
        auto f = [&](double x) {
            vector<double> w = {x, 1.0 - x};
            return value_for_weights(w);
        };
        cout << 100.0 * golden_min(f, 0.0, 1.0, 200) << '\n';
        return 0;
    }

    auto inner = [&](double y) {
        auto g = [&](double x) {
            vector<double> w = {x, y, 1.0 - x - y};
            return value_for_weights(w);
        };
        return golden_min(g, 0.0, max(0.0, 1.0 - y), 80);
    };

    cout << 100.0 * golden_min(inner, 0.0, 1.0, 80) << '\n';
    return 0;
}
