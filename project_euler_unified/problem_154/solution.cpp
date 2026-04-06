#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 154: Count triples (i,j,k) with i+j+k=200000 where
 * the trinomial coefficient IS divisible by 10^12.
 *
 * v_p(binom(n; i,j,k)) = v_p(n!) - v_p(i!) - v_p(j!) - v_p(k!)
 * Need v_2 >= 12 AND v_5 >= 12.
 */

int main() {
    const int N = 200000;
    const int TARGET = 12;

    // Precompute v_p(m!) for p = 2 and p = 5
    vector<int> v2(N + 1), v5(N + 1);
    v2[0] = v5[0] = 0;
    for (int m = 1; m <= N; m++) {
        int x = m, c2 = 0, c5 = 0;
        while (x % 2 == 0) { c2++; x /= 2; }
        x = m;
        while (x % 5 == 0) { c5++; x /= 5; }
        v2[m] = v2[m - 1] + c2;
        v5[m] = v5[m - 1] + c5;
    }

    int V2N = v2[N];
    int V5N = v5[N];

    long long count = 0;

    // Iterate i <= j <= k, i+j+k = N
    // So i <= N/3, j >= i, k = N-i-j >= j => j <= (N-i)/2
    for (int i = 0; i <= N / 3; i++) {
        // For v_p: need V_pN - v_p[i] - v_p[j] - v_p[N-i-j] < TARGET
        int r2i = V2N - v2[i]; // v2 of binom without j,k parts
        int r5i = V5N - v5[i];

        for (int j = i; j <= (N - i) / 2; j++) {
            int k = N - i - j;
            int val2 = r2i - v2[j] - v2[k];
            int val5 = r5i - v5[j] - v5[k];

            if (val2 >= TARGET && val5 >= TARGET) {
                // Count with multiplicity
                if (i == j && j == k) count += 1;
                else if (i == j || j == k) count += 3;
                else count += 6;
            }
        }
    }

    printf("%lld\n", count);
    return 0;
}
