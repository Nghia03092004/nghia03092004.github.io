/*
 * Problem 959: Sequence Alignment Score
 *
 * Compute Levenshtein (edit) distance between first 100 digits
 * of pi and e using Wagner-Fischer DP.
 *
 * Complexity: O(n^2) time and space, n = 100.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    string s1 = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";
    string s2 = "2718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427";

    int n = s1.size(), m = s2.size();
    vector<vector<int>> d(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; i++) d[i][0] = i;
    for (int j = 0; j <= m; j++) d[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i-1] == s2[j-1]) {
                d[i][j] = d[i-1][j-1];
            } else {
                d[i][j] = 1 + min({d[i-1][j], d[i][j-1], d[i-1][j-1]});
            }
        }
    }

    cout << d[n][m] << endl;
    return 0;
}
