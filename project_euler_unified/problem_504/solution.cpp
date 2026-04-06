#include <bits/stdc++.h>
using namespace std;

int main() {
    const int M = 100;

    // Precompute GCD table
    int g[M + 1][M + 1];
    for (int i = 0; i <= M; i++)
        for (int j = 0; j <= M; j++)
            g[i][j] = __gcd(i, j);

    // Precompute perfect squares
    // Max interior points: ((M+M)*(M+M))/2 + 1 = 2*M*M + 1 = 20001
    int max_i = 2 * M * M + 1;
    vector<bool> is_sq(max_i + 1, false);
    for (int k = 0; (long long)k * k <= max_i; k++)
        is_sq[k * k] = true;

    int count = 0;

    for (int a = 1; a <= M; a++) {
        for (int b = 1; b <= M; b++) {
            for (int c = 1; c <= M; c++) {
                for (int d = 1; d <= M; d++) {
                    int boundary = g[a][b] + g[b][c] + g[c][d] + g[d][a];
                    int twice_area = (a + c) * (b + d);
                    int twice_i = twice_area - boundary + 2;
                    // i must be integer, so twice_i must be even
                    if (twice_i % 2 != 0) continue;
                    int interior = twice_i / 2;
                    if (interior >= 0 && interior <= max_i && is_sq[interior])
                        count++;
                }
            }
        }
    }

    cout << count << endl;
    return 0;
}
