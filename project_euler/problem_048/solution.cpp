#include <bits/stdc++.h>
using namespace std;

long long modpow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = ((__int128)result * base) % mod;
        base = ((__int128)base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    const long long MOD = 10000000000LL;
    long long answer = 0;
    for (int k = 1; k <= 1000; k++)
        answer = (answer + modpow(k, k, MOD)) % MOD;

    printf("%010lld\n", answer);
    return 0;
}
