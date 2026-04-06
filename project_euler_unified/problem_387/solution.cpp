#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// Modular exponentiation: a^e mod m (handles large intermediates via __int128)
ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Deterministic Miller-Rabin for n < 3.317e14
bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    for (ll a : {2, 3, 5, 7, 11, 13, 17}) {
        if (a >= n) continue;
        ll x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < r - 1; i++) {
            x = (__int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const ll LIMIT = 100000000000000LL; // 10^14
    const ll PREFIX_LIMIT = LIMIT / 10;

    ll total = 0;
    // (number, digit_sum)
    vector<pair<ll, int>> frontier;
    for (int d = 1; d <= 9; d++)
        frontier.push_back({d, d});

    while (!frontier.empty()) {
        vector<pair<ll, int>> next_frontier;
        for (auto& [n, s] : frontier) {
            // Check if strong Harshad: n/s is prime
            if (n % s == 0 && is_prime(n / s)) {
                for (int d : {1, 3, 7, 9}) {
                    ll candidate = 10 * n + d;
                    if (candidate < LIMIT && is_prime(candidate))
                        total += candidate;
                }
            }
            // Expand: append digits 0..9
            for (int d = 0; d <= 9; d++) {
                ll new_n = 10 * n + d;
                int new_s = s + d;
                if (new_n < PREFIX_LIMIT && new_s > 0 && new_n % new_s == 0)
                    next_frontier.push_back({new_n, new_s});
            }
        }
        frontier = move(next_frontier);
    }

    cout << total << endl;
    return 0;
}
