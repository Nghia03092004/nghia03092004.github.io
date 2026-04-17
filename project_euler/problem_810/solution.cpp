#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000;
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    auto xor_mul = [](int a, int b) -> long long {
        long long r = 0;
        while (b) {
            if (b & 1) r ^= a;
            a <<= 1; b >>= 1;
        }
        return r;
    };
    for (int i = 2; i <= N; i++) {
        if (!is_prime[i]) continue;
        for (int j = i; j <= N; j++) {
            long long p = xor_mul(i, j);
            if (p > N) break;
            if (p > i) is_prime[p] = false;
        }
    }
    long long ans = 0;
    for (int i = 2; i <= N; i++)
        if (is_prime[i]) ans += i;
    cout << ans << endl;
    return 0;
}
