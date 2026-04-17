#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 923: Combinatorial Game Values
 *
 * Nim heaps 1..n. G(n) = XOR(1..n).
 * Count n <= 10^6 with G(n) = 0.
 *
 * XOR prefix period-4 pattern:
 *   n%4=0: n,  n%4=1: 1,  n%4=2: n+1,  n%4=3: 0
 *
 * Proof: 4k XOR (4k+1) XOR (4k+2) XOR (4k+3) = 0.
 * G(n) = 0 iff n = 3 mod 4.
 * Count = (N-3)/4 + 1 = 250000.
 */

int xor_prefix(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        case 3: return 0;
    }
    return -1;
}

int main() {
    int N = 1000000;
    int answer = (N - 3) / 4 + 1;
    cout << answer << endl;

    // Verify
    int brute = 0, xv = 0;
    for (int n = 1; n <= 10000; n++) {
        xv ^= n;
        if (xv == 0) brute++;
        assert(xv == xor_prefix(n));
    }
    assert(brute == (10000 - 3) / 4 + 1);

    return 0;
}
