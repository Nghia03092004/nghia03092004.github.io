#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

int main() {
    // St. Petersburg Lottery with finite bankroll
    // Fair entry fee F = m + B / 2^m where m = floor(log2(B))

    // B = 10^10
    ull B = 10000000000ULL;

    // m = floor(log2(B))
    int m = 0;
    ull temp = B;
    while (temp > 1) {
        temp >>= 1;
        m++;
    }
    // m = 33 since 2^33 = 8589934592 < 10^10 < 2^34

    ull pow2m = 1ULL << m;

    printf("B = %llu\n", B);
    printf("m = floor(log2(B)) = %d\n", m);
    printf("2^m = %llu\n", pow2m);

    // F = m + B / 2^m
    double F = (double)m + (double)B / (double)pow2m;
    printf("Fair entry fee F = %.8f\n", F);

    // Compute for various bankrolls
    printf("\nBankroll -> Fair Fee:\n");
    for (int e = 1; e <= 20; e++) {
        // B = 10^e (approximate with double for large e)
        double Bd = pow(10.0, e);
        int md = (int)floor(log2(Bd));
        double Fd = md + Bd / pow(2.0, md);
        printf("  B = 10^%d: m = %d, F = %.8f\n", e, md, Fd);
    }

    return 0;
}
