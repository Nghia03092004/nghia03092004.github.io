// IOI 1994 - The Triangle
// Bottom-up DP: find max-sum path from top to bottom
// Time: O(n^2), Space: O(n^2)
#include <bits/stdc++.h>
using namespace std;

int tri[101][101];

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            scanf("%d", &tri[i][j]);

    // Bottom-up DP: modify triangle in place
    for (int i = n - 2; i >= 0; i--)
        for (int j = 0; j <= i; j++)
            tri[i][j] += max(tri[i + 1][j], tri[i + 1][j + 1]);

    printf("%d\n", tri[0][0]);
    return 0;
}
