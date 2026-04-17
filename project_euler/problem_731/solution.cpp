#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 731: A Stoneham Number
 *
 * A = sum_{i=1}^{inf} 1/(3^i * 10^{3^i})
 * A(n) = 10 digits of A starting at decimal position n.
 *
 * For each term i with 3^i <= n+9, compute digits of 1/3^i at offset (n - 3^i)
 * using modular exponentiation: 10^offset mod 3^i.
 */

typedef unsigned long long ull;
typedef __int128 lll;

ull power_mod(ull base, ull exp, ull mod) {
    ull result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Get num_digits of 1/3^i starting at position 'start'
vector<int> digits_inv3i(int i, ull start, int num_digits) {
    // 3^i as big number - for i up to ~34, this fits in 128 bits
    // Use Python for the actual large computation; here demonstrate for small i
    ull mod = 1;
    for (int j = 0; j < i; j++) mod *= 3;

    ull r = power_mod(10, start, mod);
    vector<int> result;
    for (int k = 0; k < num_digits; k++) {
        ull nr = (lll)10 * r;
        result.push_back(nr / mod);
        r = nr % mod;
    }
    return result;
}

int main() {
    // Verify A(100)
    // Terms: i=1 (3^1=3), i=2 (9), i=3 (27), i=4 (81)
    int num_digits = 10;
    int extra = 20;
    ull n = 100;

    // For small cases, accumulate
    // (Full implementation needs arbitrary precision for large i)
    printf("A(10^16) requires Python/mpz for 3^34 modular arithmetic\n");

    return 0;
}
