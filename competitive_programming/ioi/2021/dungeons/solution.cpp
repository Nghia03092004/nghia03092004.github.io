#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2021 - Dungeons
// Binary lifting with strength threshold levels.
// At level k (threshold 2^k), precompute fast-forward through all opponents
// with s[i] < 2^k (guaranteed win). Use binary lifting for O(log n) per step.

const int MAXN = 400005;
const int LEVELS = 25;
const int LOG = 25;

int n;
int s_val[MAXN], p_val[MAXN], w_val[MAXN], l_val[MAXN];

// nxt[k][j][i] = destination after 2^j win-steps at level k
// gain[k][j][i] = total strength gained in those steps
int nxt[LEVELS][LOG][MAXN];
ll gain[LEVELS][LOG][MAXN];

void init(int _n, vector<int> s, vector<int> p, vector<int> w, vector<int> l) {
    n = _n;
    for (int i = 0; i < n; i++) {
        s_val[i] = s[i];
        p_val[i] = p[i];
        w_val[i] = w[i];
        l_val[i] = l[i];
    }

    for (int k = 0; k < LEVELS; k++) {
        ll threshold = 1LL << k;
        // Base case: one step at level k
        for (int i = 0; i <= n; i++) {
            if (i == n) {
                nxt[k][0][i] = n;
                gain[k][0][i] = 0;
            } else if (s_val[i] < threshold) {
                // Guaranteed win: gain s[i], go to w[i]
                nxt[k][0][i] = w_val[i];
                gain[k][0][i] = s_val[i];
            } else {
                // Cannot fast-forward: stop
                nxt[k][0][i] = i;
                gain[k][0][i] = 0;
            }
        }
        // Binary lifting
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i <= n; i++) {
                int mid = nxt[k][j - 1][i];
                nxt[k][j][i] = nxt[k][j - 1][mid];
                gain[k][j][i] = gain[k][j - 1][i] + gain[k][j - 1][mid];
            }
        }
    }
}

ll simulate(int x, int z) {
    ll strength = z;
    int dungeon = x;

    while (dungeon != n) {
        // Find highest level where we can fast-forward
        int k = 0;
        while (k < LEVELS - 1 && (1LL << (k + 1)) <= strength) k++;

        // Fast-forward through winnable opponents at this level
        bool moved = false;
        for (int j = LOG - 1; j >= 0; j--) {
            int nd = nxt[k][j][dungeon];
            ll ng = gain[k][j][dungeon];
            if (nd != dungeon) {
                if (strength + ng < (1LL << (k + 1)) || k == LEVELS - 1) {
                    strength += ng;
                    dungeon = nd;
                    moved = true;
                }
            }
        }

        if (!moved) {
            if (dungeon == n) break;
            // Handle single encounter
            if (strength >= s_val[dungeon]) {
                strength += s_val[dungeon];
                dungeon = w_val[dungeon];
            } else {
                strength += p_val[dungeon];
                dungeon = l_val[dungeon];
            }
        }
    }

    return strength;
}

int main() {
    int _n, q;
    scanf("%d %d", &_n, &q);
    vector<int> s(_n), p(_n), w(_n), l(_n);
    for (int i = 0; i < _n; i++)
        scanf("%d %d %d %d", &s[i], &p[i], &w[i], &l[i]);
    init(_n, s, p, w, l);
    while (q--) {
        int x, z;
        scanf("%d %d", &x, &z);
        printf("%lld\n", simulate(x, z));
    }
    return 0;
}
