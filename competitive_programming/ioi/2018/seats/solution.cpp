#include <bits/stdc++.h>
using namespace std;

int H, W;
int N;
vector<int> row, col; // row[i], col[i] = position of seat i

// For each 2x2 block, track the 4 seat numbers (sorted)
// A block at (r, c) covers cells (r,c), (r+1,c), (r,c+1), (r+1,c+1)
// It's "bad" for prefix k if exactly 1 or 3 of its cells are in {0..k-1}

// Segment tree: for each prefix k, store the count of bad blocks
// A 2x2 block with sorted seat numbers a < b < c < d contributes:
//   +1 to badness for k in [a+1, b] (exactly 1 cell in prefix)
//   -1 for k in [b+1, c] (exactly 2 cells)... wait, 2 cells can be bad or good.

// Actually, a 2x2 block is bad for prefix k if exactly 1 or 3 cells are in the prefix.
// Sorted: a < b < c < d.
// For k <= a: 0 cells in prefix -> not bad
// For a < k <= b: 1 cell -> bad
// For b < k <= c: 2 cells -> could be bad or not (depends on arrangement)
//   Actually, 2 cells in a 2x2 block: bad if they're diagonal (not forming a 1x2 or 2x1 rect)
//   But for the rectangle characterization, any 2x2 block with exactly 1 or 3 is bad.
//   With 2, it's fine (they could be adjacent, forming part of a rectangle boundary).
//   Hmm, actually with 2 diagonal cells in a 2x2 block, the prefix can't be a rectangle.

// Let me reconsider. The correct characterization:
// For each 2x2 block, count the cells in the prefix. Call this count c.
// The block is "bad" if c == 1 or c == 3.
// (c == 0, 2, 4 are "good".)

// Sorted seats in block: a < b < c < d.
// c_count as a function of k:
//   k <= a: 0 (good)
//   a < k <= b: 1 (bad)
//   b < k <= c: 2 (good)
//   c < k <= d: 3 (bad)
//   k > d: 4 (good)

// So block contributes to badness:
//   +1 for k in (a, b]
//   +1 for k in (c, d]
// i.e., badness[k] += 1 for k in {a+1, ..., b} and {c+1, ..., d}

// Using a segment tree with range updates and point queries,
// we can maintain badness[k] for each k.
// Then count the number of k where badness[k] == 0.

// Segment tree supporting:
// - Range add
// - Count of zeros in range

struct SegTree {
    int n;
    vector<int> mn;     // minimum in range
    vector<int> cnt;    // count of minimums in range
    vector<int> lazy;   // lazy add

    void init(int _n) {
        n = _n;
        mn.assign(4 * n, 0);
        cnt.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }

    void build(int node, int l, int r) {
        if (l == r) {
            mn[node] = 0;
            cnt[node] = 1;
            return;
        }
        int mid = (l + r) / 2;
        build(2*node, l, mid);
        build(2*node+1, mid+1, r);
        pushUp(node);
    }

    void pushUp(int node) {
        if (mn[2*node] < mn[2*node+1]) {
            mn[node] = mn[2*node];
            cnt[node] = cnt[2*node];
        } else if (mn[2*node] > mn[2*node+1]) {
            mn[node] = mn[2*node+1];
            cnt[node] = cnt[2*node+1];
        } else {
            mn[node] = mn[2*node];
            cnt[node] = cnt[2*node] + cnt[2*node+1];
        }
    }

    void pushDown(int node) {
        if (lazy[node] != 0) {
            for (int c : {2*node, 2*node+1}) {
                mn[c] += lazy[node];
                lazy[c] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update(int node, int l, int r, int ql, int qr, int val) {
        if (ql > qr || ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            mn[node] += val;
            lazy[node] += val;
            return;
        }
        pushDown(node);
        int mid = (l + r) / 2;
        update(2*node, l, mid, ql, qr, val);
        update(2*node+1, mid+1, r, ql, qr, val);
        pushUp(node);
    }

    int queryZeros(int node, int l, int r, int ql, int qr) {
        if (ql > qr || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) {
            return mn[node] == 0 ? cnt[node] : 0;
        }
        pushDown(node);
        int mid = (l + r) / 2;
        return queryZeros(2*node, l, mid, ql, qr) +
               queryZeros(2*node+1, mid+1, r, ql, qr);
    }
};

SegTree seg;

int seatAt[1000][1000]; // seatAt[r][c] = seat number at (r, c)

void addBlock(int r, int c, int sign) {
    // 2x2 block at (r, c): cells (r,c),(r+1,c),(r,c+1),(r+1,c+1)
    if (r + 1 >= H || c + 1 >= W) return;
    int vals[4] = {
        seatAt[r][c], seatAt[r+1][c], seatAt[r][c+1], seatAt[r+1][c+1]
    };
    sort(vals, vals + 4);
    int a = vals[0], b = vals[1], c_ = vals[2], d = vals[3];
    // Contribute to badness for k in [a+1, b] and [c_+1, d]
    if (a + 1 <= b)
        seg.update(1, 1, N, a + 1, b, sign);
    if (c_ + 1 <= d)
        seg.update(1, 1, N, c_ + 1, d, sign);
}

int give_initial_chart(int _H, int _W, vector<int> R, vector<int> C) {
    H = _H; W = _W;
    N = H * W;
    row = R; col = C;

    for (int i = 0; i < N; i++) {
        seatAt[row[i]][col[i]] = i;
    }

    seg.init(N + 1);
    seg.build(1, 1, N);

    // Add all 2x2 blocks
    for (int r = 0; r + 1 < H; r++) {
        for (int c = 0; c + 1 < W; c++) {
            addBlock(r, c, +1);
        }
    }

    // Also add boundary "virtual" blocks to handle edge conditions
    // (Prefix k is a rectangle iff badness[k] == 0 AND bounding box = k)
    // For simplicity, we also need to track bounding box constraints.
    // The 2x2 block approach with proper boundary handling gives exact answer.

    // Count zeros in [1, N]
    return seg.queryZeros(1, 1, N, 1, N);
}

int swap_seats(int a, int b) {
    // Remove all 2x2 blocks affected by seats a and b
    // Each seat affects up to 4 blocks: (r-1,c-1), (r-1,c), (r,c-1), (r,c)
    set<pair<int,int>> affected;
    for (int s : {a, b}) {
        int r = row[s], c = col[s];
        for (int dr = -1; dr <= 0; dr++)
            for (int dc = -1; dc <= 0; dc++)
                if (r+dr >= 0 && c+dc >= 0 && r+dr+1 < H && c+dc+1 < W)
                    affected.insert({r+dr, c+dc});
    }

    // Remove affected blocks
    for (auto [r, c] : affected) addBlock(r, c, -1);

    // Swap seats a and b
    swap(seatAt[row[a]][col[a]], seatAt[row[b]][col[b]]);
    swap(row[a], row[b]);
    swap(col[a], col[b]);

    // Re-add affected blocks
    for (auto [r, c] : affected) addBlock(r, c, +1);

    return seg.queryZeros(1, 1, N, 1, N);
}

int main() {
    int h, w, q;
    scanf("%d %d %d", &h, &w, &q);
    int n = h * w;
    vector<int> R(n), C(n);
    for (int i = 0; i < n; i++) scanf("%d %d", &R[i], &C[i]);

    printf("%d\n", give_initial_chart(h, w, R, C));
    for (int i = 0; i < q; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        printf("%d\n", swap_seats(a, b));
    }
    return 0;
}
