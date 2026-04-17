#include <bits/stdc++.h>
using namespace std;

bool is_pentagonal(long long v) {
    if (v <= 0) return false;
    long long disc = 1 + 24 * v;
    long long s = (long long)sqrt((double)disc);
    while (s * s < disc) s++;
    while (s * s > disc) s--;
    if (s * s != disc) return false;
    return (1 + s) % 6 == 0;
}

int main() {
    // Every hexagonal number is triangular: H_m = T_{2m-1}.
    // So we just iterate hexagonal numbers and check if pentagonal.
    // Start after H_143 = 40755.
    for (long long m = 144; ; m++) {
        long long h = m * (2 * m - 1);
        if (is_pentagonal(h)) {
            cout << h << endl;
            return 0;
        }
    }
    return 0;
}
