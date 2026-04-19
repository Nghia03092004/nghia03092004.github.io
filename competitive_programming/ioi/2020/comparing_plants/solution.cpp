#include <algorithm>
#include <cassert>
#include <cstdio>
#include <set>
#include <vector>

using namespace std;

constexpr int MAXN = 251000;
constexpr int SZ = 262144;

int n, K, RankVal[MAXN], w[MAXN], VPos[MAXN];

struct SegmentTree {
    int lazy[SZ + SZ], mn[SZ + SZ];

    void pull(int node) {
        mn[node] = min(mn[node * 2], mn[node * 2 + 1]);
    }

    void init(int node, int left, int right) {
        lazy[node] = 0;
        if (left == right) {
            mn[node] = RankVal[left] - 1;
            return;
        }
        int mid = (left + right) >> 1;
        init(node * 2, left, mid);
        init(node * 2 + 1, mid + 1, right);
        pull(node);
    }

    void add_node(int node, int value) {
        lazy[node] += value;
        mn[node] += value;
    }

    void push(int node) {
        add_node(node * 2, lazy[node]);
        add_node(node * 2 + 1, lazy[node]);
        lazy[node] = 0;
    }

    void add(int node, int left, int right, int ql, int qr, int value) {
        if (ql > qr) {
            return;
        }
        if (ql <= left && right <= qr) {
            add_node(node, value);
            return;
        }
        push(node);
        int mid = (left + right) >> 1;
        if (ql <= mid) {
            add(node * 2, left, mid, ql, qr, value);
        }
        if (qr > mid) {
            add(node * 2 + 1, mid + 1, right, ql, qr, value);
        }
        pull(node);
    }

    void put(int node, int left, int right, int pos, int value) {
        if (left == right) {
            mn[node] = value;
            return;
        }
        push(node);
        int mid = (left + right) >> 1;
        if (pos <= mid) {
            put(node * 2, left, mid, pos, value);
        } else {
            put(node * 2 + 1, mid + 1, right, pos, value);
        }
        pull(node);
    }

    int first_zero(int node, int left, int right) {
        if (left == right) {
            return left;
        }
        push(node);
        int mid = (left + right) >> 1;
        if (mn[node * 2] == 0) {
            return first_zero(node * 2, left, mid);
        }
        return first_zero(node * 2 + 1, mid + 1, right);
    }

    int find_zero() {
        if (mn[1] != 0) {
            return 0;
        }
        return first_zero(1, 1, n);
    }
} seg;

set<int> active_set, good;

void insert_pos(int x) {
    active_set.insert(x);
    auto it = active_set.find(x);
    int nxt = (next(it) != active_set.end()) ? *next(it) : *active_set.begin() + n;
    int prv = (it != active_set.begin()) ? *prev(it) : *prev(active_set.end()) - n;

    int mark_nxt = (nxt - 1) % n + 1;
    if (nxt - x >= K) {
        good.insert(mark_nxt);
    } else {
        good.erase(mark_nxt);
    }
    if (x - prv >= K) {
        good.insert(x);
    }
}

void erase_pos(int x) {
    auto it = active_set.find(x);
    int nxt = (next(it) != active_set.end()) ? *next(it) : *active_set.begin() + n;
    int prv = (it != active_set.begin()) ? *prev(it) : *prev(active_set.end()) - n;
    active_set.erase(it);

    good.erase(x);
    if (!active_set.empty() && nxt - prv >= K) {
        good.insert((nxt - 1) % n + 1);
    }
}

int bit[MAXN];

void add_bit(int idx, int value) {
    while (idx < MAXN) {
        bit[idx] += value;
        idx += idx & -idx;
    }
}

int sum_bit(int idx) {
    int ret = 0;
    while (idx > 0) {
        ret += bit[idx];
        idx -= idx & -idx;
    }
    return ret;
}

long long RightJump[MAXN][20], LeftJump[MAXN][20];
const long long INF = 1000000000LL;

bool right_path(int a, int b) {
    int d = (b - a + n) % n;
    for (int i = 17; i >= 0; --i) {
        if (d >= RightJump[a][i]) {
            d -= RightJump[a][i];
            a = (a + RightJump[a][i] - 1) % n + 1;
        }
    }
    return (b - a + n) % n < K && VPos[a] <= VPos[b];
}

bool left_path(int a, int b) {
    int d = (a - b + n) % n;
    for (int i = 17; i >= 0; --i) {
        if (d >= LeftJump[a][i]) {
            d -= LeftJump[a][i];
            a = (a - LeftJump[a][i] - 1 + n) % n + 1;
        }
    }
    return (a - b + n) % n < K && VPos[a] <= VPos[b];
}

bool path(int a, int b) {
    return right_path(a, b) || left_path(a, b);
}

int compare_plants(int a, int b) {
    ++a;
    ++b;
    if (path(a, b)) {
        return 1;
    }
    if (path(b, a)) {
        return -1;
    }
    return 0;
}

void build_jumps() {
    fill(bit, bit + MAXN, 0);
    for (int i = 1; i <= K; ++i) {
        add_bit(VPos[i], 1);
    }

    for (int i = 1; i <= n; ++i) {
        int left = VPos[i] + 1, right = n, best = 0;
        int target = sum_bit(VPos[i]);
        while (left <= right) {
            int mid = (left + right) >> 1;
            if (sum_bit(mid) > target) {
                best = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        if (best) {
            best = w[best];
            RightJump[i][0] = (best - i + n) % n;
        } else {
            RightJump[i][0] = INF;
        }

        add_bit(VPos[i], -1);
        add_bit(VPos[(i + K - 1) % n + 1], 1);
    }

    fill(bit, bit + MAXN, 0);
    for (int i = n; i > n - K; --i) {
        add_bit(VPos[i], 1);
    }

    for (int i = n; i >= 1; --i) {
        int left = VPos[i] + 1, right = n, best = 0;
        int target = sum_bit(VPos[i]);
        while (left <= right) {
            int mid = (left + right) >> 1;
            if (sum_bit(mid) > target) {
                best = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        if (best) {
            best = w[best];
            LeftJump[i][0] = (i - best + n) % n;
        } else {
            LeftJump[i][0] = INF;
        }

        add_bit(VPos[i], -1);
        add_bit(VPos[(i - K + n - 1) % n + 1], 1);
    }

    for (int j = 0; j < 18; ++j) {
        for (int i = 1; i <= n; ++i) {
            if (RightJump[i][j] >= INF) {
                RightJump[i][j + 1] = INF;
            } else {
                int t = (i + RightJump[i][j] - 1) % n + 1;
                RightJump[i][j + 1] = min(INF, RightJump[t][j] + RightJump[i][j]);
            }

            if (LeftJump[i][j] >= INF) {
                LeftJump[i][j + 1] = INF;
            } else {
                int t = (i - LeftJump[i][j] - 1 + n) % n + 1;
                LeftJump[i][j + 1] = min(INF, LeftJump[t][j] + LeftJump[i][j]);
            }
        }
    }
}

void init(int k, vector<int> r) {
    n = static_cast<int>(r.size());
    K = k;
    active_set.clear();
    good.clear();
    for (int i = 0; i < n; ++i) {
        RankVal[i + 1] = r[i] + 1;
    }

    seg.init(1, 1, n);
    for (int i = 1; i <= n; ++i) {
        int t;
        while ((t = seg.find_zero())) {
            insert_pos(t);
            seg.put(1, 1, n, t, INF);
        }
        assert(!good.empty());
        int a = *good.begin();
        w[i] = a;
        VPos[a] = i;
        seg.add(1, 1, n, max(a - K + 1, 1), a - 1, -1);
        seg.add(1, 1, n, a - K + 1 + n, min(a - 1 + n, n), -1);
        erase_pos(a);
    }

    for (int i = 1; i <= K; ++i) {
        add_bit(VPos[i], 1);
    }
    for (int i = 1; i <= n; ++i) {
        assert(sum_bit(VPos[i]) == RankVal[i]);
        add_bit(VPos[i], -1);
        add_bit(VPos[(i + K - 1) % n + 1], 1);
    }

    build_jumps();
}

int main() {
    int n_val, k_val, q;
    scanf("%d %d %d", &n_val, &k_val, &q);
    vector<int> r(n_val);
    for (int i = 0; i < n_val; ++i) {
        scanf("%d", &r[i]);
    }
    init(k_val, r);
    while (q--) {
        int x, y;
        scanf("%d %d", &x, &y);
        printf("%d\n", compare_plants(x, y));
    }
    return 0;
}
