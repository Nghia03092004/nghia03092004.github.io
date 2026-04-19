#include <bits/stdc++.h>
using namespace std;

namespace {

long long det3(const array<array<long long, 3>, 3>& a) {
    return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) -
           a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
           a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
}

bool solve_system(const vector<array<long long, 3>>& rows, const vector<long long>& rhs,
                  array<long long, 3>& answer) {
    array<array<long long, 3>, 3> a{};
    for (int i = 0; i < 3; ++i) {
        a[i] = rows[i];
    }
    long long det = det3(a);
    if (det == 0) {
        return false;
    }

    for (int col = 0; col < 3; ++col) {
        auto b = a;
        for (int row = 0; row < 3; ++row) {
            b[row][col] = rhs[row];
        }
        long long num = det3(b);
        if (num % det != 0) {
            return false;
        }
        answer[col] = num / det;
    }
    return true;
}

void solve() {
    const vector<array<long long, 3>> queries = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1},
        {1, 2, 3},
    };

    vector<long long> responses(5);
    for (int i = 0; i < 5; ++i) {
        cout << queries[i][0] << ' ' << queries[i][1] << ' ' << queries[i][2] << endl;
        cin >> responses[i];
    }

    for (int lie = 0; lie < 5; ++lie) {
        vector<array<long long, 3>> rows;
        vector<long long> rhs;
        for (int i = 0; i < 5; ++i) {
            if (i == lie) {
                continue;
            }
            rows.push_back(queries[i]);
            rhs.push_back(responses[i]);
        }

        array<long long, 3> answer{};
        if (!solve_system(rows, rhs, answer)) {
            continue;
        }
        if (answer[0] < 0 || answer[1] < 0 || answer[2] < 0) {
            continue;
        }

        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            if (i == lie) {
                continue;
            }
            long long value = 0;
            for (int j = 0; j < 3; ++j) {
                value += queries[i][j] * answer[j];
            }
            if (value != responses[i]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cout << answer[0] << ' ' << answer[1] << ' ' << answer[2] << endl;
            return;
        }
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
