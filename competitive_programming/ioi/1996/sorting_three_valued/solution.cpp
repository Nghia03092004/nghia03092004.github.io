// IOI 1996 - Sorting a Three-Valued Sequence
// Count misplacements between zones, resolve 2-cycles then 3-cycles
// Time: O(n), Space: O(n)
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    int a[1001];
    int cnt[4] = {}; // cnt[v] = count of value v

    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        cnt[a[i]]++;
    }

    // Zone boundaries (0-indexed)
    // Zone 1: [0, cnt[1])
    // Zone 2: [cnt[1], cnt[1]+cnt[2])
    // Zone 3: [cnt[1]+cnt[2], n)

    // c[i][j] = count of value j in zone i
    int c[4][4] = {};
    for (int i = 0; i < n; i++) {
        int zone;
        if (i < cnt[1]) zone = 1;
        else if (i < cnt[1] + cnt[2]) zone = 2;
        else zone = 3;
        c[zone][a[i]]++;
    }

    // Two-cycle swaps
    int swaps = 0;
    int two12 = min(c[1][2], c[2][1]);
    int two13 = min(c[1][3], c[3][1]);
    int two23 = min(c[2][3], c[3][2]);
    swaps += two12 + two13 + two23;

    // Remaining after two-cycles
    c[1][2] -= two12; c[2][1] -= two12;
    c[1][3] -= two13; c[3][1] -= two13;
    c[2][3] -= two23; c[3][2] -= two23;

    // Remaining misplacements form three-cycles, each needs 2 swaps
    swaps += 2 * (c[1][2] + c[1][3]);

    printf("%d\n", swaps);
    return 0;
}
