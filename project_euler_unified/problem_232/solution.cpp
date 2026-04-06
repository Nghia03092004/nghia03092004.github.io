#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 100;

    // P[a][b] = prob P2 wins when it's P2's turn, P1 needs a, P2 needs b
    // Q[a][b] = prob P2 wins when it's P1's turn
    vector<vector<double>> P(N + 1, vector<double>(N + 1, 0.0));
    vector<vector<double>> Q(N + 1, vector<double>(N + 1, 0.0));

    // Boundaries
    // P[0][b] = 0 (P1 reached 100)
    // Q[a][0] = 1 for a > 0 (P2 already won)
    for (int a = 1; a <= N; a++) {
        Q[a][0] = 1.0;
        P[a][0] = 1.0;
    }

    for (int a = 1; a <= N; a++) {
        for (int b = 1; b <= N; b++) {
            // P(a,b) = max_T [2*pt*W + (1-pt)*P(a-1,b)] / (1+pt)
            // where pt = 1/2^T, score = 2^(T-1)
            double best = 0.0;
            int score = 1;
            for (int T = 1; T < 40; T++) {
                double pt = pow(0.5, T);
                double W;
                if (b > score) {
                    W = Q[a][b - score];
                } else {
                    W = 1.0; // P2 wins
                }
                double val = (2.0 * pt * W + (1.0 - pt) * P[a - 1][b]) / (1.0 + pt);
                if (val > best) best = val;
                if (score >= N) break;
                score *= 2;
            }
            P[a][b] = best;
            Q[a][b] = 0.5 * P[a - 1][b] + 0.5 * P[a][b];
        }
    }

    printf("%.8f\n", Q[N][N]);
    return 0;
}
