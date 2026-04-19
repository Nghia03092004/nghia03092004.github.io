#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MOD = 1000002022;

int N, M;
vector<int> parent_gate;
vector<vector<int>> children;
vector<int> source_state;

// For each threshold gate: number of inputs
vector<int> num_inputs;

// Precomputed: for each threshold gate i,
// the "coefficient" of each source gate's on/off state
// This is complex for general trees.

// Simple approach: recompute from scratch each time
ll ways_on_val, ways_off_val; // for gate 0

void recompute() {
    vector<ll> ways_on(N + M), ways_off(N + M);

    // Source gates
    for (int i = N; i < N + M; i++) {
        if (source_state[i - N]) {
            ways_on[i] = 1;
            ways_off[i] = 0;
        } else {
            ways_on[i] = 0;
            ways_off[i] = 1;
        }
    }

    // Process threshold gates bottom-up
    for (int i = N - 1; i >= 0; i--) {
        ll A = 1, B = 0;
        int ci = children[i].size();
        for (int ch : children[i]) {
            ll total = (ways_on[ch] + ways_off[ch]) % MOD;
            ll newA = A * total % MOD;
            ll newB = (B * total + A % MOD * ways_on[ch]) % MOD;
            A = newA;
            B = newB;
        }
        ways_on[i] = B % MOD;
        ways_off[i] = ((ll)ci * A % MOD - B % MOD + MOD) % MOD;
    }

    ways_on_val = ways_on[0];
}

void init(int _N, int _M, vector<int> P, vector<int> A) {
    N = _N; M = _M;
    parent_gate = P;
    source_state = A;
    children.resize(N + M);
    num_inputs.resize(N, 0);

    for (int i = 1; i < N + M; i++) {
        children[P[i]].push_back(i);
        if (P[i] < N) num_inputs[P[i]]++;
    }
}

int count_ways(int L, int R) {
    // Toggle source gates L..R (0-indexed from N)
    for (int i = L; i <= R; i++)
        source_state[i - N] ^= 1;

    recompute();
    return (int)(ways_on_val % MOD);
}

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);
    vector<int> P(n + m), A(m);
    P[0] = -1;
    for (int i = 1; i < n + m; i++) scanf("%d", &P[i]);
    for (int i = 0; i < m; i++) scanf("%d", &A[i]);
    init(n, m, P, A);
    while (q--) {
        int L, R;
        scanf("%d %d", &L, &R);
        printf("%d\n", count_ways(L, R));
    }
    return 0;
}
