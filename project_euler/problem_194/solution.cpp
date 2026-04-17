#include <bits/stdc++.h>
using namespace std;

int main() {
    // P(G, n) = n^{down a} * n^{down b} / n^{down 2}
    //         = (n-2)^{down (a-2)} * n^{down b}
    // (a, b, n) = (25, 75, 1984), mod 10^8

    const long long MOD = 100000000;
    int a = 25, b = 75, n = 1984;

    // Compute (n-2)^{down (a-2)} = 1982 * 1981 * ... * 1960
    long long result = 1;
    for (int i = 0; i < a - 2; i++) {
        result = (result * ((n - 2 - i) % MOD)) % MOD;
    }

    // Multiply by n^{down b} = 1984 * 1983 * ... * 1910
    for (int i = 0; i < b; i++) {
        result = (result * ((n - i) % MOD)) % MOD;
    }

    cout << result << endl;
    return 0;
}
