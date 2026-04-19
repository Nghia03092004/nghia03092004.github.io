#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 500005;
static const int MAXNODES = MAXN * 20;

struct Node {
    int left, right, cnt;
};

Node tree[MAXNODES];
int tree_cnt;
int roots[MAXN];
int N;

int new_node() {
    tree[tree_cnt] = {0, 0, 0};
    return tree_cnt++;
}

int build(int l, int r) {
    int nd = new_node();
    if (l == r) return nd;
    int mid = (l + r) / 2;
    tree[nd].left = build(l, mid);
    tree[nd].right = build(mid + 1, r);
    return nd;
}

int update(int prev, int l, int r, int pos) {
    int nd = new_node();
    tree[nd] = tree[prev];
    tree[nd].cnt++;
    if (l == r) return nd;
    int mid = (l + r) / 2;
    if (pos <= mid) {
        tree[nd].left = update(tree[prev].left, l, mid, pos);
    } else {
        tree[nd].right = update(tree[prev].right, mid + 1, r, pos);
    }
    return nd;
}

int query(int nd, int l, int r, int ql, int qr) {
    if (ql > qr || ql > r || qr < l) return 0;
    if (ql <= l && r <= qr) return tree[nd].cnt;
    int mid = (l + r) / 2;
    return query(tree[nd].left, l, mid, ql, qr) +
           query(tree[nd].right, mid + 1, r, ql, qr);
}

int rect_count(int a_lo, int a_hi, int b_lo) {
    if (a_lo > a_hi || b_lo > N) return 0;
    a_lo = max(a_lo, 1);
    a_hi = min(a_hi, N);
    if (a_lo > a_hi) return 0;
    return query(roots[a_hi], 1, N, b_lo, N) -
           query(roots[a_lo - 1], 1, N, b_lo, N);
}

void init(int n, int A[], int B[]) {
    N = n;
    tree_cnt = 0;

    vector<pair<int, int>> students(n);
    for (int i = 0; i < n; ++i) students[i] = {A[i], B[i]};
    sort(students.begin(), students.end());

    roots[0] = build(1, N);
    int ptr = 0;
    for (int a = 1; a <= N; ++a) {
        roots[a] = roots[a - 1];
        while (ptr < n && students[ptr].first == a) {
            roots[a] = update(roots[a], 1, N, students[ptr].second);
            ++ptr;
        }
    }
}

int can(int M, int K[]) {
    vector<int> k(K, K + M);
    sort(k.begin(), k.end());
    for (int x : k) {
        if (x < 1 || x > N) return 0;
    }

    vector<long long> d(M, 0);

    auto get_k = [&](int idx) {
        return idx == -1 ? 0 : k[idx];
    };

    auto get_d = [&](int idx) -> long long {
        return idx == -1 ? 0LL : d[idx];
    };

    auto transition_value = [&](int from, int to) -> long long {
        return get_d(from) +
               rect_count(get_k(from) + 1, k[to], k[to]);
    };

    auto takeover_time = [&](int older, int newer) {
        long long rhs = get_d(newer) - get_d(older);
        if (rhs < 0) return M + 1;
        int lo = newer + 1;
        int hi = M - 1;
        int ans = M + 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            long long diff =
                rect_count(get_k(older) + 1, get_k(newer), k[mid]);
            if (diff <= rhs) {
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    };

    struct Candidate {
        int idx;
        int remove_at;
    };

    vector<Candidate> hull;
    hull.push_back({-1, M + 1});

    for (int i = 0; i < M; ++i) {
        while ((int)hull.size() >= 2 && hull.back().remove_at <= i) {
            hull.pop_back();
        }

        int best = hull.back().idx;
        d[i] = transition_value(best, i) - k[i];
        if (d[i] < 0) return 0;

        while ((int)hull.size() >= 2) {
            int prev = hull[(int)hull.size() - 2].idx;
            int last = hull.back().idx;
            int when = takeover_time(last, i);
            if (when >= hull.back().remove_at) {
                hull.pop_back();
            } else {
                break;
            }
        }

        hull.push_back({i, takeover_time(hull.back().idx, i)});
    }
    return 1;
}

int main() {
    int n;
    scanf("%d", &n);
    int *A = new int[n];
    int *B = new int[n];
    for (int i = 0; i < n; ++i) scanf("%d %d", &A[i], &B[i]);

    init(n, A, B);

    int q;
    scanf("%d", &q);
    while (q--) {
        int m;
        scanf("%d", &m);
        int *K = new int[m];
        for (int i = 0; i < m; ++i) scanf("%d", &K[i]);
        printf("%d\n", can(m, K));
        delete[] K;
    }

    delete[] A;
    delete[] B;
    return 0;
}
