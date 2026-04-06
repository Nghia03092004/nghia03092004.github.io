#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 219: Skew-cost Coding
 *
 * Build a prefix-free code for N = 10^9 symbols.
 * Bit '0' costs 1, bit '1' costs 4.
 * Minimize total cost of all codewords.
 *
 * Greedy: always split the cheapest codeword.
 * Splitting cost c -> produces c+1 and c+4.
 * Use histogram for bulk processing.
 *
 * Answer: 64564225042
 */

int main() {
    const long long N = 1000000000LL;

    // Histogram: cost -> count
    // Use a sorted map for efficient min access
    map<long long, long long> freq;
    freq[0] = 1;
    long long total_cost = 0;
    long long num_codes = 1;

    while (num_codes < N) {
        auto it = freq.begin();
        long long cost = it->first;
        long long count = it->second;
        freq.erase(it);

        // How many can we split? Each split adds 1 codeword.
        long long need = N - num_codes;
        long long splits = min(count, need);

        // Split 'splits' codewords of this cost
        total_cost += splits * (cost + 5);
        num_codes += splits;

        // Add children
        freq[cost + 1] += splits;
        freq[cost + 4] += splits;

        // If we didn't split all, put the rest back
        if (splits < count) {
            freq[cost] += (count - splits);
        }
    }

    cout << total_cost << endl;
    return 0;
}
