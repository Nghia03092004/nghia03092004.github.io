#include <bits/stdc++.h>
using namespace std;

int main() {
    // Left neighbor of 3/7 in F_{1,000,000}.
    // By Theorem 2: d = largest d <= N with d = 5 (mod 7), n = (3d-1)/7.
    // Brute-force verification scan.

    const int LIMIT = 1000000;
    long long best_n = 0, best_d = 1;

    for (int d = 1; d <= LIMIT; d++) {
        if (d % 7 == 0) continue;
        long long n = (3LL * d - 1) / 7;
        if (n * best_d > best_n * d) {
            best_n = n;
            best_d = d;
        }
    }

    cout << best_n << endl;
    return 0;
}
