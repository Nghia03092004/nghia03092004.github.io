#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 732: Standing on the Shoulders of Trolls
 *
 * N trolls in hole of depth D. Maximize total IQ of trolls that escape.
 * Troll can escape from pile of height H if H + l_i >= D.
 * Greedy: repeatedly escape highest-IQ troll that can reach surface.
 */

const long long MOD_GEN = 1000000007LL;

int main() {
    int N = 1000;
    // Generate parameters
    vector<long long> r(3*N+3);
    r[0] = 1;
    for (int i = 1; i < 3*N+3; i++)
        r[i] = r[i-1] * 5 % MOD_GEN;
    for (int i = 0; i < 3*N+3; i++)
        r[i] = r[i] % 101 + 50;

    vector<int> h(N), l(N), q(N);
    double total_h = 0;
    for (int n = 0; n < N; n++) {
        h[n] = r[3*n]; l[n] = r[3*n+1]; q[n] = r[3*n+2];
        total_h += h[n];
    }
    double D = total_h / sqrt(2.0);

    // Greedy escape
    vector<bool> escaped(N, false);
    double pile_h = total_h;
    int total_iq = 0;

    while (true) {
        int best = -1, best_q = -1;
        for (int i = 0; i < N; i++) {
            if (escaped[i]) continue;
            if (pile_h + l[i] >= D - 0.001 && q[i] > best_q) {
                best_q = q[i];
                best = i;
            }
        }
        if (best == -1) break;
        escaped[best] = true;
        pile_h -= h[best];
        total_iq += best_q;
    }

    printf("Q(%d) = %d\n", N, total_iq);
    return 0;
}
