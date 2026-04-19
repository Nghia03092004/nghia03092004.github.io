#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAXN = 500005;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int n;
long long X[MAXN], Y[MAXN];

// Segment tree: each node stores product of X (mod) and log of product
// and the best selling index in its range
struct Node {
    long long prodMod;  // product of X in range, mod
    double logProd;     // log of product of X in range
    int bestIdx;        // best selling index in range
    double bestLogVal;  // log(Y[bestIdx]) + log(product X[0..bestIdx])
                        // relative to this segment: log(Y[bestIdx]) + logProd(0..bestIdx within segment)
};

// Actually simpler: just use segment tree to maintain product of X,
// and a set of candidate indices where X[i] > 1.

// Even simpler approach: segment tree on Y * prefix_product.
// But prefix_product changes when X changes.

// Let's use the "scan from right" approach with a segment tree for products.

// Segment tree for products of X (modular and log)
long long prodMod[4 * MAXN];
double prodLog[4 * MAXN];

void build(int node, int l, int r) {
    if (l == r) {
        prodMod[node] = X[l] % MOD;
        prodLog[node] = log((double)X[l]);
        return;
    }
    int mid = (l + r) / 2;
    build(2*node, l, mid);
    build(2*node+1, mid+1, r);
    prodMod[node] = prodMod[2*node] * prodMod[2*node+1] % MOD;
    prodLog[node] = prodLog[2*node] + prodLog[2*node+1];
}

void updateX(int node, int l, int r, int pos) {
    if (l == r) {
        prodMod[node] = X[pos] % MOD;
        prodLog[node] = log((double)X[pos]);
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) updateX(2*node, l, mid, pos);
    else updateX(2*node+1, mid+1, r, pos);
    prodMod[node] = prodMod[2*node] * prodMod[2*node+1] % MOD;
    prodLog[node] = prodLog[2*node] + prodLog[2*node+1];
}

long long queryProdMod(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return 1;
    if (ql <= l && r <= qr) return prodMod[node];
    int mid = (l + r) / 2;
    return queryProdMod(2*node, l, mid, ql, qr) *
           queryProdMod(2*node+1, mid+1, r, ql, qr) % MOD;
}

double queryProdLog(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return prodLog[node];
    int mid = (l + r) / 2;
    return queryProdLog(2*node, l, mid, ql, qr) +
           queryProdLog(2*node+1, mid+1, r, ql, qr);
}

// Maintain a set of positions where X[i] > 1
set<int> candidates;

// Segment tree for max Y
long long maxY[4 * MAXN];
int maxYIdx[4 * MAXN];

void buildY(int node, int l, int r) {
    if (l == r) {
        maxY[node] = Y[l];
        maxYIdx[node] = l;
        return;
    }
    int mid = (l + r) / 2;
    buildY(2*node, l, mid);
    buildY(2*node+1, mid+1, r);
    if (maxY[2*node] >= maxY[2*node+1]) {
        maxY[node] = maxY[2*node];
        maxYIdx[node] = maxYIdx[2*node];
    } else {
        maxY[node] = maxY[2*node+1];
        maxYIdx[node] = maxYIdx[2*node+1];
    }
}

void updateY(int node, int l, int r, int pos) {
    if (l == r) {
        maxY[node] = Y[pos];
        maxYIdx[node] = pos;
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) updateY(2*node, l, mid, pos);
    else updateY(2*node+1, mid+1, r, pos);
    if (maxY[2*node] >= maxY[2*node+1]) {
        maxY[node] = maxY[2*node];
        maxYIdx[node] = maxYIdx[2*node];
    } else {
        maxY[node] = maxY[2*node+1];
        maxYIdx[node] = maxYIdx[2*node+1];
    }
}

pair<long long, int> queryMaxY(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return {-1, -1};
    if (ql <= l && r <= qr) return {maxY[node], maxYIdx[node]};
    int mid = (l + r) / 2;
    auto left = queryMaxY(2*node, l, mid, ql, qr);
    auto right = queryMaxY(2*node+1, mid+1, r, ql, qr);
    return left.first >= right.first ? left : right;
}

int solve() {
    // Candidates: positions where X[i] > 1, plus boundary at n-1
    // Scan from right. Between consecutive candidates, the best is max Y
    // in that interval. Then compare candidates using log of prefix products.

    vector<int> cands;
    // Add position 0 always as a candidate boundary
    cands.push_back(0);
    for (int x : candidates) {
        if (x > 0) cands.push_back(x);
    }
    // We consider intervals: [0, cands[0]], [cands[0]+1, cands[1]], etc.
    // Actually, the candidates partition the array. Between two consecutive
    // candidates (where X > 1), all X values are 1, so prefix product is constant.
    // In that interval, the best is just max Y.

    // Scan from right, at most ~60 candidates matter (product > 10^18)
    // Take the last ~60 candidates
    int start = max(0, (int)cands.size() - 64);

    int bestIdx = -1;
    double bestLogVal = -1e18;

    for (int ci = start; ci < (int)cands.size(); ci++) {
        int lo = cands[ci];
        int hi = (ci + 1 < (int)cands.size()) ? cands[ci + 1] - 1 : n - 1;
        auto [yval, yidx] = queryMaxY(1, 0, n - 1, lo, hi);
        // Value at yidx: Y[yidx] * prod(X[0..yidx])
        double logVal = log((double)yval) + queryProdLog(1, 0, n - 1, 0, yidx);
        if (logVal > bestLogVal) {
            bestLogVal = logVal;
            bestIdx = yidx;
        }
    }

    // If start > 0, also consider the interval [0, cands[start]-1]
    // where the product is enormous, so the best in that interval
    // might be better. But since products grow exponentially,
    // the rightmost candidate with large product dominates.
    // Actually, we need the interval [0, cands[start]-1] too.
    if (start > 0) {
        int hi = cands[start] - 1;
        auto [yval, yidx] = queryMaxY(1, 0, n - 1, 0, hi);
        double logVal = log((double)yval) + queryProdLog(1, 0, n - 1, 0, yidx);
        if (logVal > bestLogVal) {
            bestLogVal = logVal;
            bestIdx = yidx;
        }
    }

    // Compute answer modulo MOD
    long long ans = Y[bestIdx] % MOD;
    ans = ans * queryProdMod(1, 0, n - 1, 0, bestIdx) % MOD;
    return (int)ans;
}

int init(int N, int X_[], int Y_[]) {
    n = N;
    for (int i = 0; i < n; i++) { X[i] = X_[i]; Y[i] = Y_[i]; }
    build(1, 0, n - 1);
    buildY(1, 0, n - 1);
    candidates.clear();
    for (int i = 0; i < n; i++) {
        if (X[i] > 1) candidates.insert(i);
    }
    return solve();
}

int updateX(int pos, int val) {
    if (X[pos] > 1) candidates.erase(pos);
    X[pos] = val;
    if (X[pos] > 1) candidates.insert(pos);
    updateX(1, 0, n - 1, pos);
    return solve();
}

int updateY(int pos, int val) {
    Y[pos] = val;
    updateY(1, 0, n - 1, pos);
    return solve();
}

int main() {
    int N;
    scanf("%d", &N);
    int Xa[N], Ya[N];
    for (int i = 0; i < N; i++) scanf("%d", &Xa[i]);
    for (int i = 0; i < N; i++) scanf("%d", &Ya[i]);
    printf("%d\n", init(N, Xa, Ya));

    int M;
    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
        int type, pos, val;
        scanf("%d %d %d", &type, &pos, &val);
        if (type == 1) printf("%d\n", updateX(pos, val));
        else printf("%d\n", updateY(pos, val));
    }
    return 0;
}
