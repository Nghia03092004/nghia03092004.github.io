#include <bits/stdc++.h>
using namespace std;

long long pent(long long n) {
    return n * (3 * n - 1) / 2;
}

bool is_pentagonal(long long v) {
    // v is pentagonal iff 1+24v = s^2 with s = 5 mod 6,
    // equivalently (1 + s) % 6 == 0.
    if (v <= 0) return false;
    long long disc = 1 + 24 * v;
    long long s = (long long)sqrt((double)disc);
    while (s * s < disc) s++;
    while (s * s > disc) s--;
    if (s * s != disc) return false;
    return (1 + s) % 6 == 0;
}

int main() {
    // Search for minimal D = P_k - P_j with both D and P_k + P_j pentagonal.
    // Terminate when minimum gap 3k-2 >= best.
    long long best = LLONG_MAX;
    for (long long k = 2; ; k++) {
        if (3 * k - 2 >= best) break;
        long long pk = pent(k);
        for (long long j = k - 1; j >= 1; j--) {
            long long pj = pent(j);
            long long diff = pk - pj;
            if (diff >= best) break;
            if (is_pentagonal(diff) && is_pentagonal(pk + pj)) {
                best = diff;
            }
        }
    }
    cout << best << endl;
    return 0;
}
