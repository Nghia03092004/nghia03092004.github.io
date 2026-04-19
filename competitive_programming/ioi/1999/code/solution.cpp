// IOI 1999 - Code (Gray Codes)
// Generate an N-bit reflected Gray code: all 2^N binary strings such that
// consecutive strings differ in exactly one bit (cyclic).
// Approach: g(i) = i XOR (i >> 1) gives the i-th Gray code word in O(1).
// Complexity: O(2^N * N) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Edge case: N = 0 means just one empty string
    if (N == 0) {
        cout << "\n";
        return 0;
    }

    long long total = 1LL << N;

    for (long long i = 0; i < total; i++) {
        long long gray = i ^ (i >> 1);
        // Print N-bit binary representation, MSB first
        for (int bit = N - 1; bit >= 0; bit--) {
            cout << ((gray >> bit) & 1);
        }
        cout << "\n";
    }

    return 0;
}
