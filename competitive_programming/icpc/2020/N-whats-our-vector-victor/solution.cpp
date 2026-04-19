#include <bits/stdc++.h>
using namespace std;

namespace {

const long double EPS = 1e-12L;

long double dot_product(const vector<long double>& a, const vector<long double>& b) {
    long double result = 0;
    for (int i = 0; i < int(a.size()); ++i) {
        result += a[i] * b[i];
    }
    return result;
}

void solve() {
    int d, n;
    cin >> d >> n;

    vector<vector<long double>> points(n, vector<long double>(d));
    vector<long double> dist(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d; ++j) {
            cin >> points[i][j];
        }
        cin >> dist[i];
    }

    vector<vector<long double>> aug;
    aug.reserve(max(0, n - 1));
    for (int i = 1; i < n; ++i) {
        vector<long double> row(d + 1, 0);
        long double sq_norm = 0;
        for (int j = 0; j < d; ++j) {
            row[j] = points[i][j] - points[0][j];
            sq_norm += row[j] * row[j];
        }
        row[d] = (sq_norm + dist[0] * dist[0] - dist[i] * dist[i]) / 2.0L;
        aug.push_back(row);
    }

    vector<int> pivot_col;
    int row = 0;
    const int m = n - 1;
    for (int col = 0; col < d && row < m; ++col) {
        int pivot = row;
        for (int i = row + 1; i < m; ++i) {
            if (fabsl(aug[i][col]) > fabsl(aug[pivot][col])) {
                pivot = i;
            }
        }
        if (fabsl(aug[pivot][col]) <= EPS) {
            continue;
        }
        swap(aug[row], aug[pivot]);

        long double div = aug[row][col];
        for (int j = col; j <= d; ++j) {
            aug[row][j] /= div;
        }
        for (int i = 0; i < m; ++i) {
            if (i == row || fabsl(aug[i][col]) <= EPS) {
                continue;
            }
            long double factor = aug[i][col];
            for (int j = col; j <= d; ++j) {
                aug[i][j] -= factor * aug[row][j];
            }
        }
        pivot_col.push_back(col);
        ++row;
    }

    int rank = row;
    vector<int> is_pivot(d, 0);
    for (int col : pivot_col) {
        is_pivot[col] = 1;
    }

    vector<long double> y(d, 0);
    for (int i = 0; i < rank; ++i) {
        y[pivot_col[i]] = aug[i][d];
    }

    vector<vector<long double>> null_basis;
    for (int free_col = 0; free_col < d; ++free_col) {
        if (is_pivot[free_col]) {
            continue;
        }
        vector<long double> vec(d, 0);
        vec[free_col] = 1;
        for (int i = 0; i < rank; ++i) {
            vec[pivot_col[i]] = -aug[i][free_col];
        }
        null_basis.push_back(vec);
    }

    vector<vector<long double>> ortho_basis;
    for (vector<long double> vec : null_basis) {
        for (const vector<long double>& q : ortho_basis) {
            long double proj = dot_product(vec, q);
            for (int j = 0; j < d; ++j) {
                vec[j] -= proj * q[j];
            }
        }
        long double norm = sqrtl(max((long double)0, dot_product(vec, vec)));
        if (norm <= EPS) {
            continue;
        }
        for (int j = 0; j < d; ++j) {
            vec[j] /= norm;
        }
        ortho_basis.push_back(vec);
    }

    for (const vector<long double>& q : ortho_basis) {
        long double proj = dot_product(y, q);
        for (int j = 0; j < d; ++j) {
            y[j] -= proj * q[j];
        }
    }

    long double current_norm_sq = dot_product(y, y);
    long double remaining_sq = dist[0] * dist[0] - current_norm_sq;
    if (remaining_sq < 0 && remaining_sq > -1e-8L) {
        remaining_sq = 0;
    }
    if (remaining_sq > 1e-12L && !ortho_basis.empty()) {
        long double extra = sqrtl(remaining_sq);
        for (int j = 0; j < d; ++j) {
            y[j] += extra * ortho_basis[0][j];
        }
    }

    cout << fixed << setprecision(12);
    for (int j = 0; j < d; ++j) {
        if (j) {
            cout << ' ';
        }
        cout << double(points[0][j] + y[j]);
    }
    cout << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
