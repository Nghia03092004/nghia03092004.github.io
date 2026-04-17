#include <bits/stdc++.h>
using namespace std;

int main() {
    long long p = 1009, q = 3643;
    long long pm1 = p - 1, qm1 = q - 1;
    long long lambda_n = pm1 / __gcd(pm1, qm1) * qm1;

    long long total = 0;
    for (long long e = 2; e < lambda_n; e++) {
        if (__gcd(e, lambda_n) != 1LL) continue;
        if (__gcd(e - 1, pm1) != 2LL) continue;
        if (__gcd(e - 1, qm1) != 2LL) continue;
        total += e;
    }

    cout << total << endl;
    return 0;
}
