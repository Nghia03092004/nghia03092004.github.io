#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> bit;

    explicit Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}

    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    int kth(int k) const {
        int idx = 0;
        int step = 1;
        while ((step << 1) <= n) step <<= 1;
        for (; step > 0; step >>= 1) {
            int next = idx + step;
            if (next <= n && bit[next] < k) {
                idx = next;
                k -= bit[next];
            }
        }
        return idx + 1;
    }
};

struct Node {
    int l = 0;
    int r = 0;
    int parent = -1;
    vector<int> children;
};

int GetBestPosition(int N, int C, int R, int *K, int *S, int *E) {
    vector<Node> nodes(N + C);
    for (int i = 0; i < N; ++i) {
        nodes[i].l = nodes[i].r = i;
    }

    Fenwick fw(N);
    vector<int> slot_node(N), nxt(N, -1), prv(N, -1);
    for (int i = 0; i < N; ++i) {
        fw.add(i + 1, 1);
        slot_node[i] = i;
        if (i > 0) prv[i] = i - 1;
        if (i + 1 < N) nxt[i] = i + 1;
    }

    int node_cnt = N;
    for (int round = 0; round < C; ++round) {
        int left_slot = fw.kth(S[round] + 1) - 1;
        int right_slot = fw.kth(E[round] + 1) - 1;

        int cur = left_slot;
        vector<int> children;
        while (true) {
            children.push_back(slot_node[cur]);
            if (cur == right_slot) break;
            cur = nxt[cur];
        }

        int id = node_cnt++;
        nodes[id].children = children;
        nodes[id].l = nodes[children.front()].l;
        nodes[id].r = nodes[children.back()].r;
        for (int child : children) nodes[child].parent = id;

        slot_node[left_slot] = id;
        int after = nxt[right_slot];
        cur = nxt[left_slot];
        while (cur != after) {
            fw.add(cur + 1, -1);
            int next_cur = nxt[cur];
            prv[cur] = nxt[cur] = -1;
            cur = next_cur;
        }
        nxt[left_slot] = after;
        if (after != -1) prv[after] = left_slot;
    }

    int root_slot = fw.kth(1) - 1;
    int root = slot_node[root_slot];

    vector<int> strong_prefix(N, 0);
    for (int i = 0; i < N - 1; ++i) {
        strong_prefix[i + 1] = strong_prefix[i] + (K[i] > R);
    }
    strong_prefix[N - 1] = strong_prefix[N - 1];

    auto good = [&](int id) -> bool {
        return strong_prefix[nodes[id].r] - strong_prefix[nodes[id].l] == 0;
    };

    vector<int> chain(node_cnt, 0), answer(N, 0);
    vector<int> st = {root};
    chain[root] = good(root) ? 1 : 0;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int child : nodes[u].children) {
            if (child < N) {
                answer[child] = chain[u];
            } else {
                chain[child] = good(child) ? chain[u] + 1 : 0;
                st.push_back(child);
            }
        }
    }

    int best_pos = 0;
    for (int i = 1; i < N; ++i) {
        if (answer[i] > answer[best_pos]) best_pos = i;
    }
    return best_pos;
}

int main() {
    int N, C, R;
    if (!(cin >> N >> C >> R)) return 0;
    vector<int> K(max(0, N - 1)), S(C), E(C);
    for (int i = 0; i < N - 1; ++i) cin >> K[i];
    for (int i = 0; i < C; ++i) cin >> S[i] >> E[i];
    cout << GetBestPosition(N, C, R, K.data(), S.data(), E.data()) << '\n';
    return 0;
}
