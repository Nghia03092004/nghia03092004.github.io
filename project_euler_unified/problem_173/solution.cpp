#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 173: Count hollow square laminae with at most N = 10^6 tiles.
 * Lamina (a, b): outer side a, inner side b, same parity, tiles = a^2 - b^2.
 * For each a >= 3, count valid inner sides b.
 */

int main() {
    long long N = 1000000;
    long long total = 0;

    for (long long a = 3; 4 * (a - 1) <= N; a++) {
        long long b_max = a - 2;
        long long lo = a * a - N;
        long long b_min;
        if (lo <= 1) {
            b_min = (a % 2 == 0) ? 2 : 1;
        } else {
            b_min = (long long)ceil(sqrt((double)lo));
            while (b_min * b_min < lo) b_min++;
            if (b_min % 2 != a % 2) b_min++;
        }
        if (b_min > b_max) continue;
        total += (b_max - b_min) / 2 + 1;
    }

    cout << total << endl;
    return 0;
}
