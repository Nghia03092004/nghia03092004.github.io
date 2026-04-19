#include <bits/stdc++.h>
using namespace std;

// Grader functions:
void add_element(string x);
bool check_element(string x);
void compile_set();  // called between phase 1 and phase 2
void answer(int P[]);

int n;
int P[1024]; // the permutation we're trying to find

void addStrings(int l, int r, vector<int> &context) {
    if (r - l <= 1) return;
    int mid = (l + r) / 2;

    // For each i in [l, mid), add string with bit i = 1 and context bits = 1
    for (int i = l; i < mid; i++) {
        string s(n, '0');
        s[i] = '1';
        for (int c : context) s[c] = '1';
        add_element(s);
    }

    // Recurse: left half with context += [mid, r)
    vector<int> leftCtx = context;
    for (int i = mid; i < r; i++) leftCtx.push_back(i);
    addStrings(l, mid, leftCtx);

    // Right half with context += [l, mid)
    vector<int> rightCtx = context;
    for (int i = l; i < mid; i++) rightCtx.push_back(i);
    addStrings(mid, r, rightCtx);
}

void findPerm(int l, int r, vector<int> &positions, vector<int> &context) {
    if (r - l == 1) {
        P[l] = positions[0];
        return;
    }
    int mid = (l + r) / 2;

    // Determine which positions in 'positions' correspond to [l, mid)
    vector<int> leftPos, rightPos;
    for (int p : positions) {
        string s(n, '0');
        s[p] = '1';
        for (int c : context) s[c] = '1';
        if (check_element(s)) {
            leftPos.push_back(p);
        } else {
            rightPos.push_back(p);
        }
    }

    // Recurse
    vector<int> leftCtx = context;
    for (int p : rightPos) leftCtx.push_back(p);
    findPerm(l, mid, leftPos, leftCtx);

    vector<int> rightCtx = context;
    for (int p : leftPos) rightCtx.push_back(p);
    findPerm(mid, r, rightPos, rightCtx);
}

void restore_permutation(int N, int w, int r) {
    n = N;

    // Phase 1: add strings
    vector<int> emptyCtx;
    addStrings(0, n, emptyCtx);

    // Shuffle happens here
    compile_set();

    // Phase 2: determine permutation
    vector<int> allPos(n);
    iota(allPos.begin(), allPos.end(), 0);
    vector<int> emptyCtx2;
    findPerm(0, n, allPos, emptyCtx2);

    // P[i] = position where original bit i ends up
    // Actually P[i] gives us: original position i maps to output position P[i]
    // We need to invert: answer[P[i]] = i (or however the grader expects it)
    answer(P);
}

int main() {
    int N, w, r;
    scanf("%d %d %d", &N, &w, &r);
    restore_permutation(N, w, r);
    return 0;
}
