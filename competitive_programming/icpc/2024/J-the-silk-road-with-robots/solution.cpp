#include <bits/stdc++.h>
using namespace std;

namespace {

static constexpr long long kNegInf = -(1LL << 60);
static constexpr int kStates = 5;

struct Matrix {
    long long value[kStates][kStates];

    Matrix() {
        for (int i = 0; i < kStates; ++i) {
            for (int j = 0; j < kStates; ++j) {
                value[i][j] = kNegInf;
            }
        }
    }
};

Matrix multiply(const Matrix& a, const Matrix& b) {
    Matrix result;
    for (int i = 0; i < kStates; ++i) {
        for (int k = 0; k < kStates; ++k) {
            if (a.value[i][k] == kNegInf) {
                continue;
            }
            for (int j = 0; j < kStates; ++j) {
                if (b.value[k][j] == kNegInf) {
                    continue;
                }
                result.value[i][j] = max(result.value[i][j],
                                         a.value[i][k] + b.value[k][j]);
            }
        }
    }
    return result;
}

Matrix make_identity_event() {
    Matrix matrix;
    for (int i = 0; i < kStates; ++i) {
        matrix.value[i][i] = 0;
    }
    return matrix;
}

Matrix make_road(const long long distance) {
    Matrix matrix;
    matrix.value[0][0] = 0;
    matrix.value[1][1] = -distance;
    matrix.value[2][2] = -2 * distance;
    matrix.value[3][3] = -distance;
    matrix.value[4][4] = -2 * distance;
    return matrix;
}

Matrix make_robot() {
    Matrix matrix = make_identity_event();
    matrix.value[0][3] = 0;
    matrix.value[1][0] = 0;
    matrix.value[2][0] = 0;
    matrix.value[1][4] = 0;
    matrix.value[2][3] = 0;
    return matrix;
}

Matrix make_shop(const long long coins) {
    Matrix matrix;
    matrix.value[0][0] = 0;
    matrix.value[0][1] = coins;
    matrix.value[0][2] = coins;
    matrix.value[1][1] = coins;
    matrix.value[2][2] = coins;
    matrix.value[3][3] = coins;
    matrix.value[4][4] = coins;
    matrix.value[3][0] = coins;
    matrix.value[4][0] = coins;
    return matrix;
}

struct SegmentTree {
    int size = 0;
    vector<Matrix> tree;
    vector<int> positions;

    explicit SegmentTree(vector<int> coords)
        : size(static_cast<int>(coords.size())),
          tree(4 * max(1, size)),
          positions(std::move(coords)) {
        if (size > 0) {
            build(1, 0, size - 1);
        }
    }

    void build(const int node, const int left, const int right) {
        if (left == right) {
            tree[node] = make_identity_event();
            return;
        }
        const int mid = (left + right) / 2;
        build(node * 2, left, mid);
        build(node * 2 + 1, mid + 1, right);
        pull(node, left, right);
    }

    void pull(const int node, const int left, const int right) {
        const int mid = (left + right) / 2;
        Matrix merged = multiply(tree[node * 2],
                                 make_road(positions[mid + 1] - positions[mid]));
        tree[node] = multiply(merged, tree[node * 2 + 1]);
    }

    void update(const int index, const Matrix& value) {
        update(1, 0, size - 1, index, value);
    }

    void update(const int node,
                const int left,
                const int right,
                const int index,
                const Matrix& value) {
        if (left == right) {
            tree[node] = value;
            return;
        }
        const int mid = (left + right) / 2;
        if (index <= mid) {
            update(node * 2, left, mid, index, value);
        } else {
            update(node * 2 + 1, mid + 1, right, index, value);
        }
        pull(node, left, right);
    }

    long long answer() const {
        return tree[1].value[0][0];
    }
};

void solve() {
    int days;
    cin >> days;

    struct Query {
        int type = 0;
        int x = 0;
        int c = 0;
    };

    vector<Query> queries(days);
    vector<int> coords;
    coords.reserve(days);
    for (int i = 0; i < days; ++i) {
        cin >> queries[i].type >> queries[i].x;
        if (queries[i].type == 2) {
            cin >> queries[i].c;
        }
        coords.push_back(queries[i].x);
    }

    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    SegmentTree seg(coords);
    for (const Query& query : queries) {
        const int index = static_cast<int>(
            lower_bound(coords.begin(), coords.end(), query.x) - coords.begin());
        if (query.type == 1) {
            seg.update(index, make_robot());
        } else {
            seg.update(index, make_shop(query.c));
        }
        cout << seg.answer() << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
