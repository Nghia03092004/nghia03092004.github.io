#include <bits/stdc++.h>
using namespace std;

int main() {
    // Nim Square: three piles, moves are removing k^2 stones (k>=1)
    // Losing positions: G(a) XOR G(b) XOR G(c) = 0, with 0 <= a <= b <= c <= N
    // By Sprague-Grundy theorem.

    const int N = 100000;

    // Compute Grundy values
    vector<int> G(N + 1, 0);
    int max_g = 0;
    {
        vector<bool> seen(200, false);
        for (int n = 1; n <= N; n++) {
            fill(seen.begin(), seen.end(), false);
            for (int k = 1; (long long)k * k <= n; k++) {
                int gv = G[n - k * k];
                if (gv < 200) seen[gv] = true;
            }
            int mex = 0;
            while (mex < 200 && seen[mex]) mex++;
            G[n] = mex;
            max_g = max(max_g, mex);
        }
    }

    // XOR of two Grundy values can be up to next-power-of-2 minus 1
    int xor_max = 1;
    while (xor_max <= max_g) xor_max <<= 1;
    xor_max -= 1;

    // Count losing positions (a,b,c) with a <= b <= c <= N and G(a)^G(b)^G(c)=0
    // Iterate c from 0 to N, maintaining:
    //   freq[g]       = count of values in [0, c-1] with Grundy value g
    //   pair_count[v]  = count of pairs (a,b) with a<=b<=c, G(a)^G(b) = v
    // For each new c:
    //   1. New pairs (x,c) for x=0..c-1: pair_count[G(x)^G(c)] += 1 for each x
    //      = pair_count[g^G(c)] += freq[g] for all g
    //   2. Pair (c,c): pair_count[0] += 1
    //   3. freq[G(c)] += 1
    //   4. answer += pair_count[G(c)]  (triples with c as max element)

    vector<long long> freq(max_g + 1, 0);
    vector<long long> pair_count(xor_max + 1, 0);
    long long answer = 0;

    for (int c = 0; c <= N; c++) {
        int gc = G[c];

        // Add new pairs (a, c) for a in [0, c-1]
        for (int g = 0; g <= max_g; g++) {
            if (freq[g] > 0) {
                pair_count[g ^ gc] += freq[g];
            }
        }
        // Add pair (c, c)
        pair_count[0] += 1;

        // Update freq
        freq[gc] += 1;

        // Count triples with c as maximum element
        answer += pair_count[gc];
    }

    cout << answer << endl;
    return 0;
}
