#include <bits/stdc++.h>
using namespace std;

// IOI 2021 - DNA
// Given strings a, b over {A, T, C}, answer queries: minimum swaps to
// transform a[x..y] into b[x..y], or -1 if impossible.
//
// Key idea: count mismatch pairs (a[i],b[i]). Matching pairs (e.g. AC with CA)
// resolve in 1 swap (2-cycle). Remaining mismatches form 3-cycles (2 swaps each).

const int MAXN = 200005;

int ps[6][MAXN];       // prefix sums for pair types: AC,CA,AT,TA,CT,TC
int char_a[3][MAXN];   // prefix sums of character counts in a
int char_b[3][MAXN];   // prefix sums of character counts in b

void init(string a, string b) {
    int n = (int)a.size();
    auto idx = [](char c) -> int {
        if (c == 'A') return 0;
        if (c == 'T') return 1;
        return 2;
    };

    memset(ps, 0, sizeof(ps));
    memset(char_a, 0, sizeof(char_a));
    memset(char_b, 0, sizeof(char_b));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 6; k++) ps[k][i + 1] = ps[k][i];
        for (int k = 0; k < 3; k++) {
            char_a[k][i + 1] = char_a[k][i];
            char_b[k][i + 1] = char_b[k][i];
        }

        char_a[idx(a[i])][i + 1]++;
        char_b[idx(b[i])][i + 1]++;

        if (a[i] != b[i]) {
            int ai = idx(a[i]), bi = idx(b[i]);
            // Pair index: AC=0, CA=1, AT=2, TA=3, CT=4, TC=5
            if      (ai == 0 && bi == 2) ps[0][i + 1]++;
            else if (ai == 2 && bi == 0) ps[1][i + 1]++;
            else if (ai == 0 && bi == 1) ps[2][i + 1]++;
            else if (ai == 1 && bi == 0) ps[3][i + 1]++;
            else if (ai == 2 && bi == 1) ps[4][i + 1]++;
            else if (ai == 1 && bi == 2) ps[5][i + 1]++;
        }
    }
}

int get_distance(int x, int y) {
    // Feasibility: same character frequencies
    for (int k = 0; k < 3; k++)
        if (char_a[k][y + 1] - char_a[k][x] != char_b[k][y + 1] - char_b[k][x])
            return -1;

    int ac = ps[0][y + 1] - ps[0][x];
    int ca = ps[1][y + 1] - ps[1][x];
    int at = ps[2][y + 1] - ps[2][x];
    int ta = ps[3][y + 1] - ps[3][x];
    int ct = ps[4][y + 1] - ps[4][x];
    int tc = ps[5][y + 1] - ps[5][x];

    // 2-cycles: pair up matching mismatch types
    int two_cycles = min(ac, ca) + min(at, ta) + min(ct, tc);

    // Remaining after 2-cycles form 3-cycles (2 swaps each)
    int rem_ac = ac - min(ac, ca);
    int rem_at = at - min(at, ta);
    int three_cycles = rem_ac + rem_at;

    return two_cycles + 2 * three_cycles;
}

int main() {
    string a, b;
    cin >> a >> b;
    init(a, b);
    int q;
    cin >> q;
    while (q--) {
        int x, y;
        cin >> x >> y;
        cout << get_distance(x, y) << "\n";
    }
    return 0;
}
