#include <bits/stdc++.h>
using namespace std;

int main() {
    // Right-to-left construction with constraint propagation.
    // Seed with multiples of 17, extend through 13, 11, 7, 5, 3, 2.
    int primes[] = {17, 13, 11, 7, 5, 3, 2};

    // Each candidate: (digit sequence, bitmask of used digits)
    vector<pair<vector<int>, int>> candidates;

    // Step 1: seed with 3-digit multiples of 17 with distinct digits
    for (int m = 17; m < 1000; m += 17) {
        int d8 = m / 100, d9 = (m / 10) % 10, d10 = m % 10;
        if (d8 != d9 && d8 != d10 && d9 != d10) {
            int mask = (1 << d8) | (1 << d9) | (1 << d10);
            candidates.push_back({{d8, d9, d10}, mask});
        }
    }

    // Steps 2-7: extend leftward
    for (int pi = 1; pi < 7; pi++) {
        int p = primes[pi];
        vector<pair<vector<int>, int>> next;
        for (auto& [seq, mask] : candidates) {
            for (int d = 0; d <= 9; d++) {
                if (mask & (1 << d)) continue;
                int val = 100 * d + 10 * seq[0] + seq[1];
                if (val % p == 0) {
                    vector<int> new_seq = {d};
                    new_seq.insert(new_seq.end(), seq.begin(), seq.end());
                    next.push_back({new_seq, mask | (1 << d)});
                }
            }
        }
        candidates = next;
    }

    // Step 8: assign remaining digit as d1
    long long total = 0;
    for (auto& [seq, mask] : candidates) {
        for (int d = 0; d <= 9; d++) {
            if (!(mask & (1 << d))) {
                long long num = d;
                for (int x : seq) num = num * 10 + x;
                total += num;
            }
        }
    }

    cout << total << endl;
    return 0;
}
