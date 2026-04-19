// IOI 1995 - Packing Bits
// Pack groups of 8 bits into bytes, output 8 bytes per line
// Time: O(n), Space: O(n)
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    vector<int> bits(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &bits[i]);

    // Pad to multiple of 8
    while ((int)bits.size() % 8 != 0)
        bits.push_back(0);

    int numBytes = (int)bits.size() / 8;
    for (int i = 0; i < numBytes; i++) {
        int val = 0;
        for (int j = 0; j < 8; j++)
            val = val * 2 + bits[i * 8 + j];

        if (i > 0 && i % 8 == 0)
            printf("\n");
        else if (i > 0)
            printf(" ");
        printf("%d", val);
    }
    printf("\n");

    return 0;
}
