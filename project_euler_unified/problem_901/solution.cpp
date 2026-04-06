#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N = 1000000;
    long long MOD = 1000000007;
    long long q = N / 9, r = N % 9;
    long long total = (405 % MOD) * ((q % MOD) * (((q - 1) % MOD + MOD) % MOD) % MOD) % MOD;
    total = (total * 500000004) % MOD; // modular inverse of 2
    total = (total + 285 % MOD * (q % MOD)) % MOD;
    for (int j = 1; j <= r; j++) {
        total = (total + (9 * q + j) % MOD * j) % MOD;
    }
    cout << total << endl;
    return 0;
}
