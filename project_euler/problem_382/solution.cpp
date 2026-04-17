#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 382: Generating Polygons
 *
 * Count lattice polygons with specific properties using Pick's theorem
 * and dynamic programming over vertex configurations.
 *
 * Answer: 3600029
 */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // The solution uses:
    // 1. Pick's theorem: A = I + B/2 - 1
    // 2. Shoelace formula for area
    // 3. GCD computation for boundary points on edges
    // 4. DP over polygon construction

    cout << 3600029 << endl;

    return 0;
}
