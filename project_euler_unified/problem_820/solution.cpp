#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // S(n) = sum_{k=1}^{n} d_n(1/k)
    // d_n(1/k) = floor(10^n / k) mod 10
    // We need d_n(1/k): the n-th decimal digit of 1/k
    // 1/k = 0.d1 d2 d3 ... dn ...
    // d_n(1/k) = floor(10^n * (1/k)) mod 10 = floor(10^n / k) mod 10
    // But 10^n is huge for n=10^7. We use modular arithmetic:
    // 10^n mod k gives us the remainder, then floor(10^n/k) mod 10 = (10 * (10^{n-1} mod k)) / k ...
    // Actually: d_n(1/k) = floor(10 * r / k) where r = 10^{n-1} mod k
    // Because: 1/k has digits determined by long division.
    // After n-1 steps, the remainder is 10^{n-1} mod k, then d_n = floor(10*r/k)

    const int N = 10000000;

    // We need 10^{N-1} mod k for each k from 1 to N.
    // 10^{N-1} mod k can be computed using modular exponentiation for each k,
    // but that's O(N log N) total which should be feasible.

    // Alternative: use the fact that for the n-th digit of 1/k,
    // we need r = 10^{n-1} mod k, then digit = (10*r) / k.

    long long total = 0;

    for(int k = 1; k <= N; k++){
        if(k == 1){
            // 1/1 = 1.000..., fractional part = 0, so d_n = 0
            continue;
        }
        // Compute 10^{N-1} mod k using fast exponentiation
        long long exp = (long long)N - 1;
        long long base = 10 % k;
        long long r = 1;
        long long b = base;
        long long e = exp;
        while(e > 0){
            if(e & 1) r = r * b % k;
            b = b * b % k;
            e >>= 1;
        }
        // digit = (10 * r) / k
        int digit = (int)(10LL * r / k);
        total += digit;
    }

    cout << total << endl;

    return 0;
}
