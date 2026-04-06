#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

ll mod_mul(ll a, ll b, ll m) {
    return (lll)a * b % m;
}

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll mod_inv(ll a, ll m) {
    ll x, y;
    extended_gcd(a, m, x, y);
    return (x % m + m) % m;
}

int main() {
    ll N = 1504170715041707LL;
    ll M = 4503599627370517LL;
    ll N_inv = mod_inv(N, M);

    ll sum_coins = 0;
    ll min_coin = M;

    // Phase 1: brute force forward search
    ll val = 0;
    for (ll i = 1; i <= 100000000LL; i++) {
        val = (val + N) % M;
        if (val < min_coin) {
            min_coin = val;
            sum_coins += val;
            if (min_coin < 10000000LL) break;
        }
    }

    // Phase 2: search backwards from min_coin
    // For each candidate value v < min_coin, find n such that N*n = v (mod M)
    // n = v * N_inv mod M
    // v is an Eulercoin if n is less than any n that produces a value < v
    //
    // Simpler: we collect all Eulercoins by going downward.
    // We use the recursive/GCD-like approach:
    // Starting from the last known Eulercoin (value=min_coin at some index),
    // we search for smaller values by checking v = min_coin-1, min_coin-2, ...
    // Each such v appears at some index n_v. We only care about whether
    // the value v appears in the sequence - and since M is prime, every value does.
    // The key insight: once we find all Eulercoins by brute force down to ~10^7,
    // ALL values from 1 to (last_brute_force_coin - 1) are also Eulercoins.
    // This is because the sequence visits every residue, and any value smaller
    // than the current minimum encountered at ANY future index is an Eulercoin.
    // Since every value 1..min_coin-1 appears at some index, and each is smaller
    // than min_coin, they are all Eulercoins appearing after index n.
    // Wait - no, that's wrong. They need to appear in order of decreasing value
    // to be record minima.
    //
    // Correct approach: For the remaining Eulercoins, we iterate v downward
    // and track the minimum index seen. If n_v < min_index_seen, then v is
    // an Eulercoin that appears before all previously found smaller-valued coins.

    // Actually, let's think again. An Eulercoin is a value a_n that is strictly
    // less than a_1, a_2, ..., a_{n-1}. The Eulercoins form a decreasing
    // subsequence of record minima.
    //
    // After brute force gives us the last Eulercoin at value V at index I,
    // we need to find all values v < V such that the index n_v (where a_{n_v} = v)
    // satisfies n_v > I (since we already checked up to I) AND v < all a_j for j < n_v.
    // But since V was the minimum of a_1..a_I, any v < V at index n_v > I is automatically
    // less than all a_1..a_I, and we need it to also be less than all a_{I+1}..a_{n_v-1}.
    // This is complex.
    //
    // Better approach: use the Euclidean/recursive method.

    // Let's just use a clean two-directional search.
    // Forward: find Eulercoins by brute force (gives coins with large values, small indices)
    // Backward: find Eulercoins by checking v = 1, 2, 3, ... and finding their indices
    //   n_v = v * N_inv mod M. Among these, find the ones with decreasing indices.
    //   Start with v=1: n_1 = N_inv mod M. This is the LAST Eulercoin (value 1).
    //   v=2: n_2 = 2*N_inv mod M. If n_2 < n_1, then 2 is also an Eulercoin.
    //   Continue: track max_v such that n_v is decreasing.

    // Backward search: find Eulercoins with small values
    vector<pair<ll,ll>> back_coins; // (value, index)
    ll max_index = M; // we want indices smaller than this
    for (ll v = 1; v < min_coin; v++) {
        ll nv = mod_mul(v, N_inv, M);
        if (nv < max_index) {
            max_index = nv;
            back_coins.push_back({v, nv});
        }
    }

    // Add backward coins to sum
    for (auto &p : back_coins) {
        sum_coins += p.first;
    }

    cout << sum_coins << endl;
    return 0;
}
