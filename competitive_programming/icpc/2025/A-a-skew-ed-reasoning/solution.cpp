#include <bits/stdc++.h>
using namespace std;

namespace {

struct Choice {
    int empty_side;  // 0 = final left child, 1 = final right child
    int prefix_of_old;
};

struct Slice {
    int node;
    long long out_base;
    long long out_step;
    long long local_left;
    long long local_len;
};

long long ceil_div2(long long x) {
    if (x >= 0) {
        return (x + 1) / 2;
    }
    return -((-x) / 2);
}

void emit_subtree(
    const vector<int>& lc,
    const vector<int>& rc,
    const vector<int>& sz,
    const vector<Choice>& chosen,
    vector<int>& answer
) {
    vector<Slice> st;
    st.push_back({1, 0, 1, 0, sz[1]});

    while (!st.empty()) {
        Slice cur = st.back();
        st.pop_back();
        if (cur.local_len == 0 || cur.node == 0) {
            continue;
        }

        int u = cur.node;
        if (lc[u] == 0 && rc[u] == 0) {
            answer[cur.out_base] = u;
            continue;
        }

        const Choice& ch = chosen[u];
        int a_child = (ch.empty_side == 0 ? lc[u] : rc[u]);
        int b_child = (ch.empty_side == 0 ? rc[u] : lc[u]);
        int a_size = (a_child == 0 ? 0 : sz[a_child]);
        int b_size = (b_child == 0 ? 0 : sz[b_child]);
        int k = ch.prefix_of_old;

        long long L = cur.local_left;
        long long R = cur.local_left + cur.local_len;

        long long pref_l = max<long long>(0, L);
        long long pref_r = min<long long>(k, R);
        if (pref_l < pref_r && b_child != 0) {
            st.push_back({b_child, cur.out_base + cur.out_step * (pref_l - L), cur.out_step, pref_l, pref_r - pref_l});
        }

        if (L <= k && k < R) {
            answer[cur.out_base + cur.out_step * (k - L)] = u;
        }

        long long a_l = max<long long>(0, ceil_div2(L - (k + 1)));
        long long a_r = min<long long>(a_size, ceil_div2(R - (k + 1)));
        if (a_l < a_r && a_child != 0) {
            st.push_back({
                a_child,
                cur.out_base + cur.out_step * ((k + 1 - L) + 2 * a_l),
                cur.out_step * 2,
                a_l,
                a_r - a_l
            });
        }

        int suffix_size = b_size - k;
        long long b_l = max<long long>(0, ceil_div2(L - (k + 2)));
        long long b_r = min<long long>(suffix_size, ceil_div2(R - (k + 2)));
        if (b_l < b_r && b_child != 0) {
            st.push_back({
                b_child,
                cur.out_base + cur.out_step * ((k + 2 - L) + 2 * b_l),
                cur.out_step * 2,
                k + b_l,
                b_r - b_l
            });
        }
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> lc(n + 1), rc(n + 1), sz(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> lc[i] >> rc[i];
    }

    for (int i = n; i >= 1; --i) {
        sz[i] = 1;
        if (lc[i] != 0) {
            sz[i] += sz[lc[i]];
        }
        if (rc[i] != 0) {
            sz[i] += sz[rc[i]];
        }
    }

    vector<Choice> best_min(n + 1), best_max(n + 1);
    for (int u = 1; u <= n; ++u) {
        int left_size = (lc[u] == 0 ? 0 : sz[lc[u]]);
        int right_size = (rc[u] == 0 ? 0 : sz[rc[u]]);

        vector<Choice> feasible;
        if (1 <= left_size && left_size <= right_size + 1) {
            feasible.push_back({0, right_size - left_size + 1});
        }
        if (right_size <= left_size) {
            feasible.push_back({1, left_size - right_size});
        }
        if (feasible.empty()) {
            cout << "impossible\n";
            return 0;
        }

        best_min[u] = feasible[0];
        best_max[u] = feasible[0];
        for (const Choice& ch : feasible) {
            if (ch.prefix_of_old < best_min[u].prefix_of_old) {
                best_min[u] = ch;
            }
            if (ch.prefix_of_old > best_max[u].prefix_of_old) {
                best_max[u] = ch;
            }
        }
    }

    vector<int> ans_min(n), ans_max(n);
    emit_subtree(lc, rc, sz, best_min, ans_min);
    emit_subtree(lc, rc, sz, best_max, ans_max);

    for (int i = 0; i < n; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << ans_min[i];
    }
    cout << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << ans_max[i];
    }
    cout << '\n';
    return 0;
}
