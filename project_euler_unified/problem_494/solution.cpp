#include <bits/stdc++.h>
using namespace std;

int main() {
    // Collatz prefix families
    // Count distinct E/O patterns of length k among starting values [1, N]

    const int N = 10000;
    const int K = 30;

    // For each k, store the set of distinct patterns
    // Pattern encoded as a bitmask: bit i = parity at step i
    vector<unordered_set<long long>> pattern_sets(K);

    for (int n = 1; n <= N; n++) {
        long long val = n;
        long long mask = 0;
        for (int step = 0; step < K; step++) {
            if (val % 2 == 1) {
                mask |= (1LL << step);
            }
            pattern_sets[step].insert(mask);
            if (val % 2 == 0)
                val = val / 2;
            else
                val = 3 * val + 1;
        }
    }

    long long total = 0;
    for (int k = 0; k < K; k++) {
        long long fk = pattern_sets[k].size();
        total += fk;
        // cout << "F(" << k + 1 << ") = " << fk << endl;
    }

    cout << total << endl;
    return 0;
}
