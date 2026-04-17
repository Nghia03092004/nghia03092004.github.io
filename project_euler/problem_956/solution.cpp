/*
 * Problem 956: Random Matrix Determinant
 *
 * Sum |det(A)| over all 3x3 matrices A with entries in {-1, 0, 1}.
 * Exhaustive enumeration: 3^9 = 19683 matrices.
 *
 * Complexity: O(3^9) ~ O(1).
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    long long total = 0;
    map<int, int> det_count;

    // Enumerate all 3^9 = 19683 matrices
    for (int mask = 0; mask < 19683; mask++) {
        int a[3][3];
        int tmp = mask;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                a[i][j] = (tmp % 3) - 1;  // map {0,1,2} -> {-1,0,1}
                tmp /= 3;
            }
        }

        int det = a[0][0] * (a[1][1]*a[2][2] - a[1][2]*a[2][1])
                - a[0][1] * (a[1][0]*a[2][2] - a[1][2]*a[2][0])
                + a[0][2] * (a[1][0]*a[2][1] - a[1][1]*a[2][0]);

        total += abs(det);
        det_count[det]++;
    }

    // Verify total matrix count
    assert(det_count.size() > 0);
    int sum_counts = 0;
    for (auto& [d, c] : det_count) sum_counts += c;
    assert(sum_counts == 19683);

    cout << total << endl;
    return 0;
}
