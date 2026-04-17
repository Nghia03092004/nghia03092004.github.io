#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

int main() {
    // Compute C(26, n) * (2^n - n - 1) for n = 1..26, find max
    // Use __int128 to avoid overflow in intermediate computation

    // Precompute C(26, n)
    ll C[27];
    C[0] = 1;
    for (int i = 1; i <= 26; i++) {
        C[i] = C[i-1] * (27 - i) / i;
    }

    ll best = 0;
    for (int n = 1; n <= 26; n++) {
        ll euler1 = (1LL << n) - n - 1; // 2^n - n - 1
        ll pn = C[n] * euler1;
        best = max(best, pn);
    }

    cout << best << endl;
    return 0;
}
