// IOI 2003 - Comparing Substrings
// Given string S and Q queries (a, b, len), compare substrings
// S[a..a+len-1] vs S[b..b+len-1] lexicographically.
// Uses suffix array + LCP array + sparse table for O(1) per query.
// SA construction: O(N log^2 N). Queries: O(1) each.

#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    int n;
    string s;
    vector<int> sa, rank_, lcp, inv;
    vector<vector<int>> sparse;

    SuffixArray(const string& str) : s(str), n((int)str.size()) {
        buildSA();
        buildLCP();
        buildInverse();
        buildSparse();
    }

    void buildSA() {
        sa.resize(n);
        rank_.resize(n);
        vector<int> tmp(n);
        iota(sa.begin(), sa.end(), 0);

        // Initial ranking by single character
        for (int i = 0; i < n; i++) rank_[i] = s[i];

        for (int gap = 1; gap < n; gap <<= 1) {
            auto cmp = [&](int a, int b) -> bool {
                if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
                int ra = (a + gap < n) ? rank_[a + gap] : -1;
                int rb = (b + gap < n) ? rank_[b + gap] : -1;
                return ra < rb;
            };
            sort(sa.begin(), sa.end(), cmp);

            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            }
            rank_ = tmp;
            if (rank_[sa[n - 1]] == n - 1) break;
        }
    }

    // Kasai's algorithm for LCP array
    void buildLCP() {
        lcp.resize(n, 0);
        vector<int> inv_tmp(n);
        for (int i = 0; i < n; i++) inv_tmp[sa[i]] = i;

        int k = 0;
        for (int i = 0; i < n; i++) {
            if (inv_tmp[i] == 0) { k = 0; continue; }
            int j = sa[inv_tmp[i] - 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            lcp[inv_tmp[i]] = k;
            if (k > 0) k--;
        }
    }

    void buildInverse() {
        inv.resize(n);
        for (int i = 0; i < n; i++) inv[sa[i]] = i;
    }

    // Sparse table for range minimum query on LCP array
    void buildSparse() {
        int LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        sparse.assign(LOG, vector<int>(n, 0));
        for (int i = 0; i < n; i++) sparse[0][i] = lcp[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                sparse[j][i] = min(sparse[j - 1][i],
                                   sparse[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int rmq(int l, int r) {
        if (l > r) return 0;
        int k = __lg(r - l + 1);
        return min(sparse[k][l], sparse[k][r - (1 << k) + 1]);
    }

    // LCP of suffixes starting at positions a and b
    int lcpOf(int a, int b) {
        if (a == b) return n - a;
        int ra = inv[a], rb = inv[b];
        if (ra > rb) swap(ra, rb);
        return rmq(ra + 1, rb);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    cin >> S;

    // Edge case: empty string
    if (S.empty()) {
        int Q; cin >> Q;
        while (Q--) cout << "=\n";
        return 0;
    }

    SuffixArray sa(S);

    int Q;
    cin >> Q;
    while (Q--) {
        int a, b, len;
        cin >> a >> b >> len;
        a--; b--; // convert to 0-indexed

        int commonLen = sa.lcpOf(a, b);
        if (commonLen >= len) {
            cout << "=\n";
        } else {
            if (S[a + commonLen] < S[b + commonLen])
                cout << "<\n";
            else
                cout << ">\n";
        }
    }

    return 0;
}
