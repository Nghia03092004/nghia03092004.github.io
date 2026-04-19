#include <bits/stdc++.h>
using namespace std;

namespace {

int gcd_int(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int minimal_period(const string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) {
        return 1;
    }
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pi[i] = j;
    }
    int p = n - pi.back();
    return (n % p == 0 ? p : n);
}

int rotation_match(const string& source, const string& target) {
    int n = static_cast<int>(source.size());
    if (n != static_cast<int>(target.size())) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }

    string text = source + source;
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && target[i] != target[j]) {
            j = pi[j - 1];
        }
        if (target[i] == target[j]) {
            ++j;
        }
        pi[i] = j;
    }

    int j = 0;
    for (int i = 0; i < 2 * n - 1; ++i) {
        while (j > 0 && text[i] != target[j]) {
            j = pi[j - 1];
        }
        if (text[i] == target[j]) {
            ++j;
        }
        if (j == n) {
            int pos = i - n + 1;
            if (pos < n) {
                return pos;
            }
            j = pi[j - 1];
        }
    }
    return -1;
}

bool consistent_under_permutation(const vector<int>& to, const string& source, const string& target) {
    int n = static_cast<int>(to.size());
    vector<char> seen(n, false);
    unordered_map<int, int> residue_by_modulus;

    for (int i = 0; i < n; ++i) {
        if (seen[i]) {
            continue;
        }
        vector<int> cycle;
        int cur = i;
        while (!seen[cur]) {
            seen[cur] = true;
            cycle.push_back(cur);
            cur = to[cur];
        }

        string a;
        string b;
        a.reserve(cycle.size());
        b.reserve(cycle.size());
        for (int idx : cycle) {
            a.push_back(source[idx]);
            b.push_back(target[idx]);
        }

        int pos = rotation_match(a, b);
        if (pos == -1) {
            return false;
        }

        int period = minimal_period(a);
        int need = (static_cast<int>(cycle.size()) - pos) % static_cast<int>(cycle.size());
        need %= period;

        auto it = residue_by_modulus.find(period);
        if (it == residue_by_modulus.end()) {
            residue_by_modulus[period] = need;
        } else if (it->second != need) {
            return false;
        }
    }

    vector<pair<int, int>> congruences(residue_by_modulus.begin(), residue_by_modulus.end());
    for (int i = 0; i < static_cast<int>(congruences.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(congruences.size()); ++j) {
            int m1 = congruences[i].first;
            int a1 = congruences[i].second;
            int m2 = congruences[j].first;
            int a2 = congruences[j].second;
            if ((a1 - a2) % gcd_int(m1, m2) != 0) {
                return false;
            }
        }
    }
    return true;
}

vector<string> tilt_chars(const vector<string>& grid, char dir) {
    int h = static_cast<int>(grid.size());
    int w = static_cast<int>(grid[0].size());
    vector<string> next(h, string(w, '.'));

    if (dir == 'L' || dir == 'R') {
        for (int r = 0; r < h; ++r) {
            string values;
            values.reserve(w);
            for (int c = 0; c < w; ++c) {
                if (grid[r][c] != '.') {
                    values.push_back(grid[r][c]);
                }
            }
            int offset = (dir == 'L' ? 0 : w - static_cast<int>(values.size()));
            for (int i = 0; i < static_cast<int>(values.size()); ++i) {
                next[r][offset + i] = values[i];
            }
        }
    } else {
        for (int c = 0; c < w; ++c) {
            string values;
            values.reserve(h);
            for (int r = 0; r < h; ++r) {
                if (grid[r][c] != '.') {
                    values.push_back(grid[r][c]);
                }
            }
            int offset = (dir == 'U' ? 0 : h - static_cast<int>(values.size()));
            for (int i = 0; i < static_cast<int>(values.size()); ++i) {
                next[offset + i][c] = values[i];
            }
        }
    }
    return next;
}

vector<vector<int>> tilt_labels(const vector<vector<int>>& grid, char dir) {
    int h = static_cast<int>(grid.size());
    int w = static_cast<int>(grid[0].size());
    vector<vector<int>> next(h, vector<int>(w, -1));

    if (dir == 'L' || dir == 'R') {
        for (int r = 0; r < h; ++r) {
            vector<int> values;
            values.reserve(w);
            for (int c = 0; c < w; ++c) {
                if (grid[r][c] != -1) {
                    values.push_back(grid[r][c]);
                }
            }
            int offset = (dir == 'L' ? 0 : w - static_cast<int>(values.size()));
            for (int i = 0; i < static_cast<int>(values.size()); ++i) {
                next[r][offset + i] = values[i];
            }
        }
    } else {
        for (int c = 0; c < w; ++c) {
            vector<int> values;
            values.reserve(h);
            for (int r = 0; r < h; ++r) {
                if (grid[r][c] != -1) {
                    values.push_back(grid[r][c]);
                }
            }
            int offset = (dir == 'U' ? 0 : h - static_cast<int>(values.size()));
            for (int i = 0; i < static_cast<int>(values.size()); ++i) {
                next[offset + i][c] = values[i];
            }
        }
    }
    return next;
}

vector<string> apply_sequence(vector<string> grid, const string& sequence) {
    for (char dir : sequence) {
        grid = tilt_chars(grid, dir);
    }
    return grid;
}

string corner_cycle(const string& seq) {
    bool left = (seq.find('L') != string::npos);
    bool up = (seq.find('U') != string::npos);
    if (left && up) {
        return "RDLU";
    }
    if (!left && up) {
        return "DLUR";
    }
    if (!left && !up) {
        return "LURD";
    }
    return "ULDR";
}

bool same_grid(const vector<string>& a, const vector<string>& b) {
    return a == b;
}

bool same_multiset(const vector<string>& a, const vector<string>& b) {
    array<int, 26> ca{};
    array<int, 26> cb{};
    for (const string& row : a) {
        for (char ch : row) {
            if (ch != '.') {
                ++ca[ch - 'a'];
            }
        }
    }
    for (const string& row : b) {
        for (char ch : row) {
            if (ch != '.') {
                ++cb[ch - 'a'];
            }
        }
    }
    return ca == cb;
}

bool check_cycle(const vector<string>& base_grid, const vector<string>& target, const string& order) {
    int h = static_cast<int>(base_grid.size());
    int w = static_cast<int>(base_grid[0].size());

    int tiles = 0;
    vector<vector<int>> labels(h, vector<int>(w, -1));
    string source;
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (base_grid[r][c] != '.') {
                labels[r][c] = tiles++;
                source.push_back(base_grid[r][c]);
            }
        }
    }

    vector<vector<vector<int>>> states(5);
    states[0] = labels;
    for (int i = 0; i < 4; ++i) {
        states[i + 1] = tilt_labels(states[i], order[i]);
    }

    vector<int> permutation(tiles);
    int idx = 0;
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (states[4][r][c] != -1) {
                permutation[states[4][r][c]] = idx++;
            }
        }
    }

    for (int step = 0; step < 4; ++step) {
        string pulled(tiles, '?');
        bool valid = true;
        for (int r = 0; r < h && valid; ++r) {
            for (int c = 0; c < w; ++c) {
                bool occupied = (states[step][r][c] != -1);
                bool want = (target[r][c] != '.');
                if (occupied != want) {
                    valid = false;
                    break;
                }
                if (occupied) {
                    pulled[states[step][r][c]] = target[r][c];
                }
            }
        }
        if (valid && consistent_under_permutation(permutation, source, pulled)) {
            return true;
        }
    }
    return false;
}

void solve() {
    int h, w;
    cin >> h >> w;

    vector<string> start(h), target(h);
    for (int i = 0; i < h; ++i) {
        cin >> start[i];
    }
    string blank;
    getline(cin, blank);
    getline(cin, blank);
    for (int i = 0; i < h; ++i) {
        cin >> target[i];
    }

    if (!same_multiset(start, target)) {
        cout << "no\n";
        return;
    }

    if (same_grid(start, target)) {
        cout << "yes\n";
        return;
    }

    vector<char> moves = {'L', 'R', 'U', 'D'};
    for (char dir : moves) {
        if (same_grid(tilt_chars(start, dir), target)) {
            cout << "yes\n";
            return;
        }
    }

    if (h == 1 || w == 1) {
        cout << "no\n";
        return;
    }

    const vector<string> seeds = {"LU", "LD", "RU", "RD", "UL", "UR", "DL", "DR"};
    for (const string& seq : seeds) {
        vector<string> base = apply_sequence(start, seq);
        if (check_cycle(base, target, corner_cycle(seq))) {
            cout << "yes\n";
            return;
        }
    }

    cout << "no\n";
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
