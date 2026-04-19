// IOI 1996 - Magic Squares
// BFS on permutations of 2x4 grid with 3 operations (A, B, C)
// State space: 8! = 40320, Time: O(8! * 3)
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// State: 8 positions
// Top row left-to-right: s[0..3], Bottom row left-to-right: s[4..7]
// Problem layout: 1 2 3 4  (top)
//                 8 7 6 5  (bottom, read right-to-left in problem)
struct State {
    int s[8];
};

ll encode(const State& st) {
    ll v = 0;
    for (int i = 0; i < 8; i++)
        v = v * 10 + st.s[i];
    return v;
}

// Operation A: swap top and bottom rows
// Top becomes bottom, bottom becomes top
State opA(const State& st) {
    State r;
    r.s[0] = st.s[4]; r.s[1] = st.s[5]; r.s[2] = st.s[6]; r.s[3] = st.s[7];
    r.s[4] = st.s[0]; r.s[5] = st.s[1]; r.s[6] = st.s[2]; r.s[7] = st.s[3];
    return r;
}

// Operation B: circular right shift each row
State opB(const State& st) {
    State r;
    r.s[0] = st.s[3]; r.s[1] = st.s[0]; r.s[2] = st.s[1]; r.s[3] = st.s[2];
    r.s[4] = st.s[7]; r.s[5] = st.s[4]; r.s[6] = st.s[5]; r.s[7] = st.s[6];
    return r;
}

// Operation C: clockwise rotation of center 2x2 block
// Center block positions: s[1], s[2] (top), s[5], s[6] (bottom)
// Clockwise: top-left <- bottom-left, top-right <- top-left,
//            bottom-right <- top-right, bottom-left <- bottom-right
State opC(const State& st) {
    State r = st;
    r.s[1] = st.s[5];
    r.s[2] = st.s[1];
    r.s[6] = st.s[2];
    r.s[5] = st.s[6];
    return r;
}

int main() {
    // Initial state: 1 2 3 4 / 8 7 6 5
    State init;
    init.s[0] = 1; init.s[1] = 2; init.s[2] = 3; init.s[3] = 4;
    init.s[4] = 8; init.s[5] = 7; init.s[6] = 6; init.s[7] = 5;

    // Read target
    State target;
    for (int i = 0; i < 8; i++)
        scanf("%d", &target.s[i]);

    ll initCode = encode(init);
    ll targetCode = encode(target);

    if (initCode == targetCode) {
        printf("0\n\n");
        return 0;
    }

    // BFS
    unordered_map<ll, pair<ll, char>> parent;
    unordered_map<ll, bool> visited;
    queue<State> q;
    q.push(init);
    visited[initCode] = true;

    while (!q.empty()) {
        State cur = q.front(); q.pop();
        ll curCode = encode(cur);

        State nexts[3] = {opA(cur), opB(cur), opC(cur)};
        char ops[3] = {'A', 'B', 'C'};

        for (int i = 0; i < 3; i++) {
            ll nc = encode(nexts[i]);
            if (!visited[nc]) {
                visited[nc] = true;
                parent[nc] = {curCode, ops[i]};
                if (nc == targetCode) {
                    // Reconstruct path
                    string path;
                    ll c = targetCode;
                    while (c != initCode) {
                        path += parent[c].second;
                        c = parent[c].first;
                    }
                    reverse(path.begin(), path.end());
                    printf("%d\n%s\n", (int)path.size(), path.c_str());
                    return 0;
                }
                q.push(nexts[i]);
            }
        }
    }

    return 0;
}
