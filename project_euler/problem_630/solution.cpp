#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 630: Crossed Lines
 *
 * Generate points via s_{n+1} = s_n^2 mod 50515093, s_0 = 290797.
 * Form segments from consecutive point pairs.
 * Count proper crossings using orientation tests.
 */

ll ccw(ll ax, ll ay, ll bx, ll by, ll cx, ll cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

bool crosses(ll ax, ll ay, ll bx, ll by, ll cx, ll cy, ll dx, ll dy) {
    ll d1 = ccw(ax, ay, cx, cy, dx, dy);
    ll d2 = ccw(bx, by, cx, cy, dx, dy);
    ll d3 = ccw(ax, ay, bx, by, cx, cy);
    ll d4 = ccw(ax, ay, bx, by, dx, dy);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;
    return false;
}

int main() {
    const ll MOD = 50515093;
    int n_points = 2500;  // adjust as needed

    // Generate points
    vector<ll> x(n_points), y(n_points);
    ll s = 290797;
    for (int i = 0; i < n_points; i++) {
        x[i] = s;
        s = s * s % MOD;
        y[i] = s;
        s = s * s % MOD;
    }

    // Form segments
    int n_seg = n_points / 2;
    ll count = 0;
    for (int i = 0; i < n_seg; i++) {
        for (int j = i + 1; j < n_seg; j++) {
            if (crosses(x[2*i], y[2*i], x[2*i+1], y[2*i+1],
                       x[2*j], y[2*j], x[2*j+1], y[2*j+1]))
                count++;
        }
    }
    cout << "Crossings: " << count << endl;
    return 0;
}
