#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;

/*
 * Problem 265: Binary Circles
 *
 * Place 2^5 = 32 bits in a circle, all 5-bit subsequences distinct.
 * Encode as number starting from 00000. Sum all encodings.
 *
 * Backtracking approach: build the sequence bit by bit.
 *
 * Answer: 209110240768
 */

const int N = 5;
const int L = 1 << N;  // 32
const int MASK = L - 1; // 0x1F

ll total_sum;

void dfs(ll seq, int pos, uint used, int window) {
    if (pos == L) {
        // Check wrap-around windows
        uint u = used;
        int w = window;
        for (int i = 0; i < N - 1; i++) {
            int bit = (int)((seq >> (L - 1 - i)) & 1);
            w = ((w << 1) | bit) & MASK;
            if (u & (1u << w)) return;
            u |= (1u << w);
        }
        total_sum += seq;
        return;
    }

    for (int bit = 0; bit <= 1; bit++) {
        int nw = ((window << 1) | bit) & MASK;
        if (!(used & (1u << nw))) {
            dfs((seq << 1) | bit, pos + 1, used | (1u << nw), nw);
        }
    }
}

int main() {
    total_sum = 0;
    dfs(0, N, 1u, 0);  // first N bits = 0, window 0 used
    cout << total_sum << endl;
    return 0;
}
