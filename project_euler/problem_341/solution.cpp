#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 341: Golomb's Self-Describing Sequence
 *
 * G(1) = 1, G(n) = 1 + G(n - G(G(n-1))) for n >= 2.
 * Value v appears G(v) times.  P(v) = sum_{i=1}^{v} G(i) is the last
 * position with value <= v.  G(n) = v iff P(v-1) < n <= P(v).
 *
 * Build blocks until P(v) >= (10^6-1)^3, then answer each query G(d^3)
 * by a linear scan through the blocks.
 *
 * Answer: 56098610614277014
 */

int main(){
    const long long LIMIT = 1000000;
    const long long cubicLimit = (LIMIT - 1) * (LIMIT - 1) * (LIMIT - 1);

    // Build Golomb sequence: G[v] and P[v]
    vector<long long> G = {0, 1};
    vector<long long> P = {0, 1};
    G.reserve(12000000);
    P.reserve(12000000);

    for (long long v = 2; P.back() < cubicLimit; v++) {
        long long g = 1 + G[v - G[G[v - 1]]];
        G.push_back(g);
        P.push_back(P.back() + g);
    }

    // Answer queries G(d^3) for d = 1..999999 via linear scan
    long long total = 0;
    long long idx = 1;
    for (long long d = 1; d < LIMIT; d++) {
        long long n = d * d * d;
        while (P[(size_t)idx] < n)
            idx++;
        total += idx;
    }

    cout << total << endl;
    return 0;
}
