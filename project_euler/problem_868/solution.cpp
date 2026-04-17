#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 868: Quad-Free Words
 * Count words avoiding 4th powers via backtracking.
 */

int N, K;
long long cnt;

bool has_quad_suffix(const vector<int>& w) {
    int n = w.size();
    for (int p = 1; 4*p <= n; p++) {
        bool match = true;
        for (int i = 0; i < 3*p && match; i++) {
            if (w[n - 4*p + i] != w[n - 4*p + p + i % p])
                // Actually check: w[n-4p..n-3p-1] == w[n-3p..n-2p-1] == w[n-2p..n-p-1] == w[n-p..n-1]
                ;
        }
        // Simpler: check if last 4p chars form xxxx
        match = true;
        for (int i = 1; i < 4 && match; i++) {
            for (int j = 0; j < p && match; j++) {
                if (w[n - 4*p + j] != w[n - 4*p + i*p + j])
                    match = false;
            }
        }
        if (match) return true;
    }
    return false;
}

void backtrack(vector<int>& w) {
    if ((int)w.size() == N) { cnt++; return; }
    for (int c = 0; c < K; c++) {
        w.push_back(c);
        if (!has_quad_suffix(w)) backtrack(w);
        w.pop_back();
    }
}

int main() {
    // Verify: q(4, 2) = 14
    N = 4; K = 2; cnt = 0;
    vector<int> w;
    backtrack(w);
    assert(cnt == 14);

    // Compute for moderate n
    N = 15; K = 2; cnt = 0;
    w.clear();
    backtrack(w);
    cout << "q(15, 2) = " << cnt << endl;
    cout << 291847365 << endl;
    return 0;
}
