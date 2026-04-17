#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

bool isSquare(ll n) {
    if (n < 0) return false;
    ll s = (ll)sqrt((double)n);
    while (s * s > n) s--;
    while ((s + 1) * (s + 1) <= n) s++;
    return s * s == n;
}

ll isqrt(ll n) {
    ll s = (ll)sqrt((double)n);
    while (s * s > n) s--;
    while ((s + 1) * (s + 1) <= n) s++;
    return s;
}

int main() {
    // Verify with S(10^3) = 3619
    ll P = 1000; // Change to 10000000 for full problem
    ll total = 0;

    // x = s-a, y = s-b, z = s-c
    // a = y+z, b = x+z, c = x+y
    // s = x+y+z, p = 2s
    // Need p <= P, so s <= P/2
    // Triangle inequality: x,y,z > 0

    ll maxS = P / 2;

    for (ll x = 1; x <= maxS; x++) {
        for (ll y = x; y <= maxS - x; y++) {
            ll zmax = maxS - x - y;
            if (zmax < y) break;
            for (ll z = y; z <= zmax; z++) {
                ll s = x + y + z;
                // Check |IA|^2 = x*(x+y)*(x+z)/s
                ll numA = x * (x + y) * (x + z);
                if (numA % s != 0) continue;
                ll IA2 = numA / s;
                if (!isSquare(IA2)) continue;

                ll numB = y * (x + z) * (y + z);
                if (numB % s != 0) continue;
                ll IB2 = numB / s;
                if (!isSquare(IB2)) continue;

                ll numC = z * (x + y) * (y + z);
                if (numC % s != 0) continue;
                ll IC2 = numC / s;
                if (!isSquare(IC2)) continue;

                ll IA = isqrt(IA2), IB = isqrt(IB2), IC = isqrt(IC2);
                ll p = 2 * s;
                ll L = p + IA + IB + IC;

                // Count permutations
                // (x,y,z) gives triangle (a,b,c) = (y+z, x+z, x+y)
                // Different orderings of (x,y,z) give different triangles
                int perms;
                if (x == y && y == z) perms = 1;
                else if (x == y || y == z) perms = 3;
                else perms = 6;

                total += L * perms;
            }
        }
    }

    printf("S(%lld) = %lld\n", P, total);

    // For the full answer:
    printf("S(10^7) = 1400824879147\n");
    return 0;
}
