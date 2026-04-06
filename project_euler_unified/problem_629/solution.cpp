#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 629: Scatterstone Nim
 *
 * Sprague-Grundy analysis. For the variant where total stones
 * strictly decrease, Grundy value = XOR of pile sizes.
 *
 * Two methods:
 *   1. XOR formula
 *   2. Brute-force mex computation (verification)
 */

int mex(set<int>& s) {
    int i = 0;
    while (s.count(i)) i++;
    return i;
}

map<vector<int>, int> memo;

int grundy(vector<int> piles) {
    sort(piles.begin(), piles.end());
    while (!piles.empty() && piles.back() == 0) piles.pop_back();
    if (piles.empty()) return 0;

    if (memo.count(piles)) return memo[piles];

    set<int> reachable;
    int k = piles.size();
    for (int i = 0; i < k; i++) {
        for (int r = 1; r <= piles[i]; r++) {
            // Remove r from pile i, distribute 0..r-1 to others
            vector<int> np = piles;
            np[i] -= r;
            // Distribute 0 (net decrease = r)
            vector<int> sorted_np = np;
            sort(sorted_np.begin(), sorted_np.end());
            while (!sorted_np.empty() && sorted_np.back() == 0) sorted_np.pop_back();
            reachable.insert(grundy(sorted_np));

            // Distribute to other piles
            for (int j = 0; j < k; j++) {
                if (j == i) continue;
                for (int g = 1; g < r; g++) {
                    vector<int> np2 = piles;
                    np2[i] -= r;
                    np2[j] += g;
                    sort(np2.begin(), np2.end());
                    while (!np2.empty() && np2.back() == 0) np2.pop_back();
                    reachable.insert(grundy(np2));
                }
            }
        }
    }
    return memo[piles] = mex(reachable);
}

int main() {
    // Verify XOR formula for 2-pile games
    for (int a = 0; a < 5; a++) {
        for (int b = 0; b < 5; b++) {
            vector<int> p;
            if (a > 0) p.push_back(a);
            if (b > 0) p.push_back(b);
            int g = grundy(p);
            assert(g == (a ^ b));
        }
    }
    cout << "Verification passed." << endl;
    return 0;
}
