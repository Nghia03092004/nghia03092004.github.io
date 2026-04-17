#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

int main() {
    ll N = 10000000000000000LL; // 10^16

    // S(N) = sum_{n=1}^{N} F(n)
    // F(n) = floor(log2(n)) - v2(n)
    // S(N) = A(N) - B(N)
    // A(N) = sum of floor(log2(n)) for n=1..N
    // B(N) = sum of v2(n) for n=1..N

    // Compute A(N) = sum of floor(log2(n)) for n=1..N
    int B = 63 - __builtin_clzll(N); // floor(log2(N))
    // sum_{b=0}^{B-1} b * 2^b = (B-2)*2^B + 2
    // Plus B * (N - 2^B + 1) for numbers from 2^B to N

    // Use __int128 to avoid overflow
    lll powerB = (lll)1 << B;
    lll A = (lll)(B - 2) * powerB + 2 + (lll)B * ((lll)N - powerB + 1);

    // Compute B(N) = sum_{k=1}^{inf} floor(N/2^k)
    lll Bval = 0;
    for (int k = 1; k <= B; k++) {
        Bval += (lll)N >> k;
    }

    lll result = A - Bval;

    // Print __int128
    ll r = (ll)result;
    cout << r << endl;

    return 0;
}
