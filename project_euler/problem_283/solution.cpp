#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

int main() {
    ll total = 0;

    for (ll k = 1; k <= 1000; k++) {
        ll k2 = k * k;

        // Case 1: even perimeter, x,y,z positive integers
        // xyz = 4k^2(x+y+z), x <= y <= z
        // z = 4k^2(x+y) / (xy - 4k^2)
        ll c = 4 * k2;
        ll x_max = (ll)sqrt(3.0 * c) + 2;
        for (ll x = 1; x <= x_max; x++) {
            ll y_start = max(x, c / x + 1);
            // y upper bound from z >= y
            // y <= (c + sqrt(c^2 + c*x^2)) / x
            double disc = (double)c * ((double)c + (double)x * x);
            ll y_end = (ll)((c + sqrt(disc)) / x) + 2;

            for (ll y = y_start; y <= y_end; y++) {
                ll d = x * y - c;
                if (d <= 0) continue;
                ll n = c * (x + y);
                if (n % d != 0) continue;
                ll z = n / d;
                if (z < y) break;
                total += 2 * (x + y + z);
            }
        }

        // Case 2: odd perimeter, X,Y,Z odd positive integers
        // XYZ = 16k^2(X+Y+Z), X <= Y <= Z
        // Z = 16k^2(X+Y) / (XY - 16k^2)
        ll c2 = 16 * k2;
        ll X_max = (ll)sqrt(3.0 * c2) + 2;
        for (ll X = 1; X <= X_max; X += 2) {
            ll Y_start = max(X, c2 / X + 1);
            if (Y_start % 2 == 0) Y_start++;

            double disc2 = (double)c2 * ((double)c2 + (double)X * X);
            ll Y_end = (ll)((c2 + sqrt(disc2)) / X) + 2;

            for (ll Y = Y_start; Y <= Y_end; Y += 2) {
                ll d = X * Y - c2;
                if (d <= 0) continue;
                ll n = c2 * (X + Y);
                if (n % d != 0) continue;
                ll Z = n / d;
                if (Z < Y) break;
                if (Z % 2 == 0) continue;
                total += X + Y + Z;  // P = X+Y+Z
            }
        }
    }

    cout << total << endl;
    return 0;
}
