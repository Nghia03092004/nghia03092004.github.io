#include <bits/stdc++.h>
using namespace std;

// Grader-provided function
void answer(vector<int> C, vector<int> X, vector<int> Y);

static vector<int> Xv, Yv;
static const int DUMMY = INT_MIN;

static int newSwitch(int x, int y) {
    Xv.push_back(x);
    Yv.push_back(y);
    return -(int)Xv.size();
}

// Build binary switch tree for leafOut[lo..hi-1].
// Returns DUMMY if entire subtree is dummies, else a switch ID or trigger number.
static int buildTree(int lo, int hi, const vector<int>& leafOut) {
    if (hi - lo == 1) return leafOut[lo];
    int mid = (lo + hi) / 2;
    int left  = buildTree(lo, mid, leafOut);
    int right = buildTree(mid, hi, leafOut);
    if (left == DUMMY && right == DUMMY) return DUMMY;
    return newSwitch(left, right);
}

void create_circuit(int M, vector<int> A) {
    int N = A.size();

    // Need leaves for A[0..N-1] plus halt (output 0) = N+1 real visits.
    // Pad to next power of 2 with dummy visits that loop back to root.
    int need = N + 1;
    int k = 0;
    while ((1 << k) < need) k++;
    int leaves = 1 << k;
    int numDummy = leaves - need;

    // visitOutput[i] = output of the i-th chronological leaf visit
    vector<int> visitOutput(leaves);
    for (int i = 0; i < numDummy; i++) visitOutput[i] = DUMMY;
    for (int i = 0; i < N; i++) visitOutput[numDummy + i] = A[i];
    visitOutput[leaves - 1] = 0; // halt

    // Leaf at tree position p is visited at time bitReverse(p, k).
    // So leafOutput[p] = visitOutput[bitReverse(p, k)].
    auto bitReverse = [](int x, int bits) -> int {
        int r = 0;
        for (int i = 0; i < bits; i++) { r = (r << 1) | (x & 1); x >>= 1; }
        return r;
    };

    vector<int> leafOutput(leaves);
    for (int p = 0; p < leaves; p++)
        leafOutput[p] = visitOutput[bitReverse(p, k)];

    // Build tree, then replace DUMMY references with root switch
    Xv.clear(); Yv.clear();
    int root = buildTree(0, leaves, leafOutput);
    int S = (int)Xv.size();

    for (int i = 0; i < S; i++) {
        if (Xv[i] == DUMMY) Xv[i] = root;
        if (Yv[i] == DUMMY) Yv[i] = root;
    }

    // Renumber so root switch becomes -1 (index 0 in arrays).
    // Currently root = -S. Remap: -i -> -(S - i + 1).
    auto remap = [&](int x) -> int {
        if (x <= -1 && x >= -S) return -(S - (-x) + 1);
        return x;
    };
    for (int i = 0; i < S; i++) {
        Xv[i] = remap(Xv[i]);
        Yv[i] = remap(Yv[i]);
    }
    reverse(Xv.begin(), Xv.end());
    reverse(Yv.begin(), Yv.end());

    // C[0] = -1 (root switch); C[i] = -1 for all triggers (return to root)
    vector<int> C(M + 1, -1);
    answer(C, Xv, Yv);
}

// --- Local testing stub (remove for grader submission) ---
#ifdef LOCAL_TEST
void answer(vector<int> C, vector<int> X, vector<int> Y) {
    int S = X.size();
    vector<int> state(S, 0);
    vector<int> visited;
    int cur = C[0], steps = 0;
    while (cur != 0 && steps < 100000) {
        steps++;
        if (cur > 0) { visited.push_back(cur); cur = C[cur]; }
        else {
            int idx = -cur - 1;
            cur = state[idx] == 0 ? X[idx] : Y[idx];
            state[idx] ^= 1;
        }
    }
    printf("Visited:");
    for (int x : visited) printf(" %d", x);
    printf("\nS = %d, halt = %s, reset = %s\n", S,
           cur == 0 ? "OK" : "FAIL",
           all_of(state.begin(), state.end(), [](int s){return s==0;}) ? "OK" : "FAIL");
}

int main() {
    int M, N; scanf("%d %d", &M, &N);
    vector<int> A(N);
    for (int i = 0; i < N; i++) scanf("%d", &A[i]);
    create_circuit(M, A);
}
#endif
