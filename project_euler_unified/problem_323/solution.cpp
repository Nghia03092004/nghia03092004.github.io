#include <bits/stdc++.h>
using namespace std;

int main() {
    // E[N] = sum_{k=0}^{inf} [1 - (1 - 2^{-k})^32]
    // Converges very fast. Compute directly by summing enough terms.

    long double result = 0.0L;
    for (int k = 0; k <= 200; k++) {
        long double p = 1.0L - powl(2.0L, -(long double)k);
        long double p32 = powl(p, 32.0L);
        result += 1.0L - p32;
    }

    cout << fixed << setprecision(10) << result << endl;
    return 0;
}
