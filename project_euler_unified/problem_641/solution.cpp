#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 641: A Flea on a Chess Board
 *
 * Colour sequence on the torus Z/(2W) x Z/(2H) has period
 * L = lcm(2W/gcd(dx,2W), 2H/gcd(dy,2H)).
 * Enumerate one period, then scale via division with remainder.
 */

ll gcd_(ll a, ll b) { return b ? gcd_(b, a % b) : a; }
ll lcm_(ll a, ll b) { return a / gcd_(a, b) * b; }

pair<ll, ll> count_colours(ll W, ll H, ll dx, ll dy, ll N) {
    ll Lx = 2 * W / gcd_(dx, 2 * W);
    ll Ly = 2 * H / gcd_(dy, 2 * H);
    ll L = lcm_(Lx, Ly);

    // Enumerate one full period
    vector<ll> prefix(L + 1, 0);
    ll x = 0, y = 0;
    for (ll k = 0; k < L; k++) {
        ll colour = (x / W + y / H) % 2;
        prefix[k + 1] = prefix[k] + (1 - colour);
        x = (x + dx) % (2 * W);
        y = (y + dy) % (2 * H);
    }

    ll w = prefix[L];
    ll full = (N + 1) / L;
    ll rem = (N + 1) % L;
    ll white_total = full * w + prefix[rem];
    ll black_total = (N + 1) - white_total;
    return {white_total, black_total};
}

int main() {
    auto [w, b] = count_colours(2, 3, 3, 4, 999);
    cout << "Problem 641: white=" << w << " black=" << b << endl;
    return 0;
}
