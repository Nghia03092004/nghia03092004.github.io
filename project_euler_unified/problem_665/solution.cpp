#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 665: Proportionate Nim
 *
 * Compute Sprague-Grundy values for Nim variant.
 * From pile n, remove floor(n/k) stones for k=1,2,...
 * G(n) = mex{G(n - floor(n/k)) : k >= 1, floor(n/k) > 0}
 */

int main() {
    const int N = 1000;
    vector<int> grundy(N + 1, 0);

    for (int n = 1; n <= N; n++) {
        set<int> reachable;
        // Remove floor(n/k) for k=1..n
        int prev_removal = -1;
        for (int k = 1; k <= n; k++) {
            int removal = n / k;
            if (removal == 0) break;
            if (removal == prev_removal) continue;
            prev_removal = removal;
            reachable.insert(grundy[n - removal]);
        }
        int mex = 0;
        while (reachable.count(mex)) mex++;
        grundy[n] = mex;
    }

    printf("Grundy values 0..20:");
    for (int i = 0; i <= 20; i++) printf(" %d", grundy[i]);
    printf("\n");

    // Count P-positions
    int p_count = 0;
    for (int i = 0; i <= N; i++)
        if (grundy[i] == 0) p_count++;
    printf("P-positions up to %d: %d\n", N, p_count);

    return 0;
}
