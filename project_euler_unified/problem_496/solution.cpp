#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

bool is_perfect_square(ll n) {
    if (n < 0) return false;
    if (n == 0) return true;
    ll r = (ll)sqrt((double)n);
    // Check neighborhood due to floating point
    for (ll x = max(0LL, r - 2); x <= r + 2; x++) {
        if (x * x == n) return true;
    }
    return false;
}

int main() {
    int N = 100;

    int count = 0;
    for (int a = 1; a <= N; a++) {
        for (int b = a; b <= N; b++) {
            int c_max = min(N, a + b - 1);
            for (int c = b; c <= c_max; c++) {
                // s2 = 2s = a+b+c
                ll s2 = a + b + c;
                ll sa2 = s2 - 2*a;
                ll sb2 = s2 - 2*b;
                ll sc2 = s2 - 2*c;

                if (sa2 <= 0 || sb2 <= 0 || sc2 <= 0) continue;

                ll abc = (ll)a * b * c;
                ll prod_s = sa2 * sb2 * sc2;

                ll numer = abc * (abc - prod_s);
                ll denom = s2 * prod_s;

                if (numer <= 0) continue;

                ll g = __gcd(numer, denom);
                numer /= g;
                denom /= g;

                if (is_perfect_square(numer) && is_perfect_square(denom)) {
                    count++;
                }
            }
        }
    }

    cout << count << endl;
    return 0;
}
