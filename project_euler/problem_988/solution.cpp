#include <bits/stdc++.h>
using namespace std;
const long long MOD = 1e9+7;
long long power(long long b, long long e, long long m) {
    long long r=1; b%=m;
    while(e>0){if(e&1)r=r*b%m;b=b*b%m;e>>=1;}
    return r;
}
int main() {
    int n = 10; long long p = 2;
    long long result = 1;
    for (int k = 0; k < n; k++) {
        long long factor = (power(p, n, MOD) - power(p, k, MOD) + MOD) % MOD;
        result = result * factor % MOD;
    }
    cout << result << endl;
    return 0;
}
