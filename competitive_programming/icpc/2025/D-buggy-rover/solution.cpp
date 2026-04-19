#include <bits/stdc++.h>
using namespace std;

namespace {

const int INF = (int)1e9;
const int DR[4] = {-1, 0, 1, 0};
const int DC[4] = {0, 1, 0, -1};

int dir_id(char c) {
    if (c == 'N') return 0;
    if (c == 'E') return 1;
    if (c == 'S') return 2;
    return 3;
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;
    vector<string> board(r);
    for (int i = 0; i < r; ++i) {
        cin >> board[i];
    }
    string path;
    cin >> path;

    int sr = -1, sc = -1;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (board[i][j] == 'S') {
                sr = i;
                sc = j;
            }
        }
    }

    vector<array<int, 4>> perm;
    array<int, 4> p = {0, 1, 2, 3};
    do {
        perm.push_back(p);
    } while (next_permutation(p.begin(), p.end()));
    int P = (int)perm.size();

    vector<array<int, 4>> rank(P);
    for (int id = 0; id < P; ++id) {
        for (int pos = 0; pos < 4; ++pos) {
            rank[id][perm[id][pos]] = pos;
        }
    }

    int m = (int)path.size();
    vector<vector<char>> good(m, vector<char>(P, 0));
    int cr = sr, cc = sc;
    for (int step = 0; step < m; ++step) {
        int want = dir_id(path[step]);
        bool can[4];
        for (int d = 0; d < 4; ++d) {
            int nr = cr + DR[d];
            int nc = cc + DC[d];
            can[d] = (0 <= nr && nr < r && 0 <= nc && nc < c && board[nr][nc] != '#');
        }

        for (int id = 0; id < P; ++id) {
            if (!can[want]) {
                continue;
            }
            bool ok = true;
            for (int d = 0; d < 4; ++d) {
                if (d == want || !can[d]) {
                    continue;
                }
                if (rank[id][d] < rank[id][want]) {
                    ok = false;
                    break;
                }
            }
            good[step][id] = ok;
        }

        cr += DR[want];
        cc += DC[want];
    }

    vector<int> dp(P, INF), ndp(P, INF);
    for (int id = 0; id < P; ++id) {
        if (good[0][id]) {
            dp[id] = 0;
        }
    }

    for (int step = 1; step < m; ++step) {
        fill(ndp.begin(), ndp.end(), INF);
        for (int to = 0; to < P; ++to) {
            if (!good[step][to]) {
                continue;
            }
            for (int from = 0; from < P; ++from) {
                if (dp[from] == INF) {
                    continue;
                }
                ndp[to] = min(ndp[to], dp[from] + (from != to));
            }
        }
        dp.swap(ndp);
    }

    cout << *min_element(dp.begin(), dp.end()) << '\n';
    return 0;
}
