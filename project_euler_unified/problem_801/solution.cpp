#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;
long long power(long long b, long long e, long long m) {
    long long r = 1; b %= m;
    while (e > 0) { if (e & 1) r = r * b % m; b = b * b % m; e >>= 1; }
    return r;
}
int main() {
    const int LIM = 200;
    vector<bool> sieve(LIM, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i < LIM; i++)
        if (sieve[i]) for (int j = i*i; j < LIM; j += i) sieve[j] = false;
    long long ans = 0;
    for (int p = 2; p < LIM; p++) {
        if (!sieve[p]) continue;
        for (int x = 1; x < p; x++)
            for (int y = x+1; y < p; y++)
                if (power(x, y, p) == power(y, x, p))
                    ans = (ans + x + y) % MOD;
    }
    cout << ans << endl;
    return 0;
}
