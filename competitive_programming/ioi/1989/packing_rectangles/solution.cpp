// IOI 1989 - Problem 1: Packing Rectangles
// Find smallest enclosing rectangle for 4 rectangles (each may be rotated 90 degrees).
// Enumerate all 6 layout types x 24 permutations x 16 rotations.
#include <bits/stdc++.h>
using namespace std;

int bestArea, bestW, bestH;

void updateBest(int w, int h) {
    if (w <= 0 || h <= 0) return;
    int area = w * h;
    if (area < bestArea || (area == bestArea && w + h < bestW + bestH)) {
        bestArea = area;
        // Normalize so bestW <= bestH
        bestW = min(w, h);
        bestH = max(w, h);
    }
}

void tryLayouts(int w[], int h[]) {
    // Type 1: all four in a row
    updateBest(w[0] + w[1] + w[2] + w[3],
               max({h[0], h[1], h[2], h[3]}));

    // Type 2: three stacked on one side, one beside them
    for (int i = 0; i < 4; i++) {
        int sumH = 0, maxW = 0;
        for (int j = 0; j < 4; j++) {
            if (j == i) continue;
            sumH += h[j];
            maxW = max(maxW, w[j]);
        }
        updateBest(maxW + w[i], max(sumH, h[i]));
    }

    // Type 3: two stacked left, two stacked right (3 pairings)
    int pairs[3][4] = {{0,1,2,3}, {0,2,1,3}, {0,3,1,2}};
    for (auto& p : pairs) {
        int a = p[0], b = p[1], c = p[2], d = p[3];
        updateBest(max(w[a], w[b]) + max(w[c], w[d]),
                   max(h[a] + h[b], h[c] + h[d]));
    }

    // Types 4-5: enumerate all 24 permutations for 2x2-style layouts
    int idx[4];
    int perm[4] = {0, 1, 2, 3};
    do {
        int a = perm[0], b = perm[1], c = perm[2], d = perm[3];

        // Type 4: a,b stacked left; c right of a; d right of b
        updateBest(max(w[a] + w[c], w[b] + w[d]),
                   max(h[a] + h[b], h[c] + h[d]));

        // Type 5: a left column, (b,c) stacked middle, d right column
        updateBest(w[a] + max(w[b], w[c]) + w[d],
                   max({h[a], h[b] + h[c], h[d]}));
    } while (next_permutation(perm, perm + 4));
}

int main() {
    int W[4], H[4];
    for (int i = 0; i < 4; i++)
        scanf("%d%d", &W[i], &H[i]);

    bestArea = INT_MAX;
    bestW = bestH = INT_MAX;

    // Try all 2^4 = 16 rotation states
    for (int mask = 0; mask < 16; mask++) {
        int w[4], h[4];
        for (int i = 0; i < 4; i++) {
            if (mask & (1 << i)) {
                w[i] = H[i]; h[i] = W[i];
            } else {
                w[i] = W[i]; h[i] = H[i];
            }
        }
        tryLayouts(w, h);
    }

    printf("%d\n%d %d\n", bestArea, bestW, bestH);
    return 0;
}
