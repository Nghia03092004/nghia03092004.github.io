// IOI 2002 - XOR
// Find the largest rectangle in an M x N binary grid with XOR = 1.
// Uses 2D prefix XOR and iterates over all row pairs.
// For fixed rows r1, r2: Q[c] = P[r2][c] ^ P[r1-1][c].
// Need Q[c2] ^ Q[c1-1] = 1, i.e., Q[c2] != Q[c1-1].
// Track first occurrence of 0 and 1 in Q to maximize width.
// Complexity: O(M^2 * N) time, O(M * N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N;
    cin >> M >> N;

    vector<vector<int>> a(M + 1, vector<int>(N + 1, 0));
    for (int i = 1; i <= M; i++)
        for (int j = 1; j <= N; j++)
            cin >> a[i][j];

    // 2D prefix XOR
    vector<vector<int>> P(M + 1, vector<int>(N + 1, 0));
    for (int i = 1; i <= M; i++)
        for (int j = 1; j <= N; j++)
            P[i][j] = a[i][j] ^ P[i - 1][j] ^ P[i][j - 1] ^ P[i - 1][j - 1];

    int bestArea = 0;

    // Enumerate all pairs of rows
    for (int r1 = 1; r1 <= M; r1++) {
        for (int r2 = r1; r2 <= M; r2++) {
            int height = r2 - r1 + 1;

            // Q[c] = P[r2][c] ^ P[r1-1][c]
            // Need Q[c2] != Q[c1-1] to get XOR=1, maximize c2 - (c1-1)
            int first[2] = {-1, -1};
            first[0] = 0; // Q[0] = 0 always

            for (int c = 1; c <= N; c++) {
                int qc = P[r2][c] ^ P[r1 - 1][c];
                if (first[qc] == -1) first[qc] = c;

                // Want Q[c1-1] != qc, with c1-1 as small as possible
                int need = 1 - qc;
                if (first[need] != -1) {
                    int width = c - first[need];
                    bestArea = max(bestArea, height * width);
                }
            }
        }
    }

    cout << bestArea << "\n";

    return 0;
}
