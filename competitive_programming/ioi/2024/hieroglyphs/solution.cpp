#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

void clean(vi& a, vi& b) {
    vi ap, bp;
    set<int> as, bs;
    for (int x : a) as.insert(x);
    for (int x : b) bs.insert(x);
    for (int x : a) if (bs.count(x)) ap.push_back(x);
    for (int x : b) if (as.count(x)) bp.push_back(x);
    swap(a, ap);
    swap(b, bp);
}

map<int, int> coordinate_compress(vi& a, vi& b) {
    int cc = 0;
    map<int, int> mp;
    map<int, int> rmp;
    for (int& x : a) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    for (int& x : b) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    return rmp;
}

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < static_cast<int>(b.size()) && b[j] == x) {
            ++j;
        }
    }
    return j == static_cast<int>(b.size());
}

vector<int> get_candidate(vector<int> a, vector<int> b) {
    int n = a.size();
    int m = b.size();

    vi occ_a(max(n, m) + 1, 0), occ_b(max(n, m) + 1, 0);
    for (int x : a) occ_a[x]++;
    for (int x : b) occ_b[x]++;

    vi c;
    queue<int> qa, qb;
    for (int i = 0; i < n; ++i) {
        if (occ_a[a[i]] <= occ_b[a[i]]) {
            qa.push(i);
        }
    }
    for (int i = 0; i < m; ++i) {
        if (occ_a[b[i]] > occ_b[b[i]]) {
            qb.push(i);
        }
    }

    int ia_cur = 0, ib_cur = 0, ia_next = 0, ib_next = 0;
    vi occ_a_cur = occ_a, occ_a_next = occ_a;
    vi occ_b_cur = occ_b, occ_b_next = occ_b;

    while (!qa.empty() && !qb.empty()) {
        while (ia_next < qa.front()) {
            occ_a_next[a[ia_next]]--;
            ia_next++;
        }
        while (ib_next < qb.front()) {
            occ_b_next[b[ib_next]]--;
            ib_next++;
        }

        int x = a[ia_next];
        int y = b[ib_next];
        int occ_x = occ_a_next[x];
        int occ_y = occ_b_next[y];

        bool a_good = (occ_a_next[y] >= occ_y && occ_b_cur[x] > occ_b_next[x]);
        bool b_good = (occ_b_next[x] >= occ_x && occ_a_cur[y] > occ_a_next[y]);

        if (a_good && b_good) return {};
        if (!a_good && !b_good) return {};

        if (a_good) {
            c.push_back(x);
            qa.pop();
            while (ia_cur <= ia_next) {
                occ_a_cur[a[ia_cur]]--;
                ia_cur++;
            }
            while (b[ib_cur] != x) {
                occ_b_cur[b[ib_cur]]--;
                ib_cur++;
            }
            occ_b_cur[b[ib_cur]]--;
            ib_cur++;
        } else {
            c.push_back(y);
            qb.pop();
            while (ib_cur <= ib_next) {
                occ_b_cur[b[ib_cur]]--;
                ib_cur++;
            }
            while (a[ia_cur] != y) {
                occ_a_cur[a[ia_cur]]--;
                ia_cur++;
            }
            occ_a_cur[a[ia_cur]]--;
            ia_cur++;
        }
    }

    while (!qa.empty()) {
        c.push_back(a[qa.front()]);
        qa.pop();
    }
    while (!qb.empty()) {
        c.push_back(b[qb.front()]);
        qb.pop();
    }

    return (is_subsequence(a, c) && is_subsequence(b, c)) ? c : vi();
}

vector<int> index_vector(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();
    vi v(m);
    int max_v = 0;
    for (int x : a) max_v = max(max_v, x);
    for (int x : b) max_v = max(max_v, x);

    vi prev_occ_b(max_v + 1, -1);
    vvi a_occ(max_v + 1);
    for (int i = 0; i < n; ++i) a_occ[a[i]].push_back(i);
    for (int i = 0; i <= max_v; ++i) a_occ[i].push_back(n);

    vector<pair<int, int>> min_stack;
    for (int i = 0; i < m; ++i) {
        if (prev_occ_b[b[i]] == -1) {
            v[i] = a_occ[b[i]][0];
        } else {
            int min_val = lower_bound(min_stack.begin(), min_stack.end(),
                                      pair<int, int>(prev_occ_b[b[i]], -1))->second;
            if (min_val < n) {
                v[i] = *lower_bound(a_occ[b[i]].begin(), a_occ[b[i]].end(), min_val + 1);
            } else {
                v[i] = n;
            }
        }
        while (!min_stack.empty() && min_stack.back().second >= v[i]) {
            min_stack.pop_back();
        }
        min_stack.emplace_back(i, v[i]);
        prev_occ_b[b[i]] = i;
    }
    return v;
}

template <class T>
struct RMQ {
    vector<vector<T>> jmp;

    explicit RMQ(const vector<T>& v) : jmp(1, v) {
        for (int pw = 1, k = 1; pw * 2 <= static_cast<int>(v.size()); pw *= 2, ++k) {
            jmp.emplace_back(static_cast<int>(v.size()) - pw * 2 + 1);
            for (int j = 0; j < static_cast<int>(jmp[k].size()); ++j) {
                jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
            }
        }
    }

    T query(int a, int b) {
        int dep = 31 - __builtin_clz(b - a);
        return min(jmp[dep][a], jmp[dep][b - (1 << dep)]);
    }
};

bool no_single_crossing(const vi& a, const vi& b, const vi& c) {
    int n = a.size();
    int m = b.size();
    int l = c.size();

    vi rb = b;
    reverse(rb.begin(), rb.end());
    vi rc = c;
    reverse(rc.begin(), rc.end());

    vi v_ab = index_vector(a, b);
    vi v_rbc = index_vector(rb, rc);
    RMQ<int> rmq(v_rbc);

    vi v_max_rc_i(n + 1);
    v_max_rc_i[n] = 0;
    for (int i = n - 1; i >= 0; --i) {
        v_max_rc_i[i] = v_max_rc_i[i + 1];
        if (a[i] == rc[v_max_rc_i[i + 1]]) {
            v_max_rc_i[i]++;
        }
        if (v_max_rc_i[i] > l) v_max_rc_i[i] = l;
    }

    vi v_min_rc_i(m + 1);
    v_min_rc_i[0] = l - 1;
    for (int i = 0; i < m; ++i) {
        v_min_rc_i[i + 1] = v_min_rc_i[i];
        if (b[i] == rc[v_min_rc_i[i]]) {
            v_min_rc_i[i + 1]--;
        }
        if (v_min_rc_i[i] < -1) v_min_rc_i[i] = -1;
    }

    for (int j = 0; j < m; ++j) {
        int ai = v_ab[j];
        if (ai == n) continue;
        int min_rc_i = v_min_rc_i[j + 1];
        int max_rc_i = v_max_rc_i[ai + 1];
        if (min_rc_i + 1 < max_rc_i &&
            rmq.query(min_rc_i + 1, max_rc_i) + j < m - 1) {
            return false;
        }
    }
    return true;
}

bool verify(const vi& a, const vi& b, const vi& c) {
    if (c.empty()) return false;
    return no_single_crossing(a, b, c) && no_single_crossing(b, a, c);
}

vector<int> ucs(vector<int> a, vector<int> b) {
    clean(a, b);
    if (a.empty() || b.empty()) {
        return {};
    }
    map<int, int> rev = coordinate_compress(a, b);
    vi c = get_candidate(a, b);
    if (verify(a, b, c)) {
        for (int& x : c) x = rev[x];
        return c;
    }
    return {-1};
}

int main() {
    int n, m;
    scanf("%d", &n);
    vector<int> A(n);
    for (int i = 0; i < n; ++i) scanf("%d", &A[i]);
    scanf("%d", &m);
    vector<int> B(m);
    for (int i = 0; i < m; ++i) scanf("%d", &B[i]);

    vector<int> res = ucs(A, B);
    if (res.size() == 1 && res[0] == -1) {
        printf("-1\n");
    } else {
        printf("%d\n", (int)res.size());
        for (int i = 0; i < (int)res.size(); ++i) {
            printf("%d%c", res[i], " \n"[i + 1 == (int)res.size()]);
        }
        if (res.empty()) {
            printf("\n");
        }
    }
    return 0;
}
