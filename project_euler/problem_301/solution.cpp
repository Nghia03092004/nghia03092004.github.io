#include <cstdio>

/*
 * Problem 301: Nim
 *
 * (n, 2n, 3n) is a P-position iff n XOR 2n XOR 3n = 0,
 * equivalently n has no two consecutive 1-bits in binary.
 * Count of such n in [1, 2^30] = F(32) = 2178309.
 */

int main() {
    long long a = 1, b = 1;
    for (int i = 3; i <= 32; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    printf("%lld\n", b);
    return 0;
}
