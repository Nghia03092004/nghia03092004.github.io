#include <bits/stdc++.h>
using namespace std;

int main() {
    // streak(n) = k: smallest k s.t. n+k NOT div by k+1
    // streak(n) >= s: n ≡ 1 (mod lcm(2,...,s))
    // P(s,N) = floor((N-2)/lcm(2,...,s)) - floor((N-2)/lcm(2,...,s+1))
    // Answer = sum_{i=1}^{31} P(i, 4^i)

    typedef __int128 lll;

    auto my_gcd = [](lll a, lll b) -> lll {
        while (b) { a %= b; swap(a, b); }
        return a;
    };

    // L[k] = lcm(2, 3, ..., k)
    vector<lll> L(34, 1);
    lll cur = 1;
    for (int k = 2; k <= 33; k++) {
        cur = cur / my_gcd(cur, (lll)k) * k;
        L[k] = cur;
    }

    lll ans = 0;
    for (int i = 1; i <= 31; i++) {
        lll N = 1;
        for (int j = 0; j < i; j++) N *= 4;
        lll Nm2 = N - 2;

        lll c1;
        if (i == 1) {
            c1 = Nm2;
        } else {
            c1 = (L[i] <= Nm2) ? Nm2 / L[i] : 0;
        }
        lll c2 = (L[i+1] <= Nm2) ? Nm2 / L[i+1] : 0;

        ans += c1 - c2;
    }

    // Print __int128
    long long result = (long long)ans;
    cout << result << endl;

    return 0;
}
