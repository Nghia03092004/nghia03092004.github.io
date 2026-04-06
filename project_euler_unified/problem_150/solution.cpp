#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 150: Minimum-sum Sub-triangle
 * Generate triangular array via LCG, use prefix sums, enumerate all sub-triangles.
 * Complexity: O(N^3/6) ~ 1.67e8 for N=1000.
 */

int main() {
    const int N = 1000;
    const long long MOD = 1 << 20;

    // Generate triangular array
    vector<vector<long long>> tri(N);
    long long t = 0;
    for (int r = 0; r < N; r++) {
        tri[r].resize(r + 1);
        for (int c = 0; c <= r; c++) {
            t = (615949LL * t + 797807LL) % MOD;
            tri[r][c] = t - (1 << 19);
        }
    }

    // Prefix sums per row
    vector<vector<long long>> prefix(N);
    for (int r = 0; r < N; r++) {
        prefix[r].resize(r + 2, 0);
        for (int c = 0; c <= r; c++) {
            prefix[r][c + 1] = prefix[r][c] + tri[r][c];
        }
    }

    // Find minimum sub-triangle sum
    long long ans = LLONG_MAX;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c <= r; c++) {
            long long triSum = 0;
            for (int h = 0; r + h < N; h++) {
                triSum += prefix[r + h][c + h + 1] - prefix[r + h][c];
                if (triSum < ans) ans = triSum;
            }
        }
    }

    assert(ans == -271248680LL);
    cout << ans << endl;
    return 0;
}
