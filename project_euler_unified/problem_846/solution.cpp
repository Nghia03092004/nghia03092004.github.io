#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 846: Magic Bracelets
 *
 * A bracelet uses beads with values in {1, 2, p^k, 2p^k} for odd primes p.
 * Two adjacent beads a,b must satisfy: ab = x^2 + 1 for some integer x.
 * All beads are distinct. Bracelet length >= 3.
 * F(N) = sum of potencies of all magic bracelets using values <= N.
 * Rotations and reflections are equivalent.
 *
 * We build a graph and enumerate simple cycles.
 */

typedef long long ll;

// Check if n is of the form x^2 + 1
bool is_sum_sq_plus_one(ll n) {
    if (n < 1) return false;
    ll x = (ll)sqrt((double)(n - 1));
    for (ll t = max(0LL, x - 2); t <= x + 2; t++) {
        if (t * t + 1 == n) return true;
    }
    return false;
}

// Check if n is a valid bead value: 1, 2, p^k, or 2p^k for odd prime p
bool is_valid_bead(int n) {
    if (n == 1 || n == 2) return true;
    int m = n;
    int factor_of_2 = 0;
    while (m % 2 == 0) { m /= 2; factor_of_2++; }
    if (factor_of_2 > 1) return false;
    // m must be a prime power p^k with p odd prime
    if (m == 1) return false; // n was a power of 2 > 2
    // Find the smallest prime factor
    int p = 0;
    for (int i = 3; (ll)i * i <= m; i += 2) {
        if (m % i == 0) { p = i; break; }
    }
    if (p == 0) return true; // m is prime
    // Check m is a power of p
    while (m % p == 0) m /= p;
    return m == 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int N = 1000000;

    // Generate valid bead values
    vector<int> beads;
    for (int i = 1; i <= N; i++) {
        if (is_valid_bead(i)) {
            beads.push_back(i);
        }
    }

    int sz = beads.size();
    // Map bead value to index
    unordered_map<int, int> bead_idx;
    for (int i = 0; i < sz; i++) {
        bead_idx[beads[i]] = i;
    }

    // Build adjacency list
    vector<vector<int>> adj(sz);
    for (int i = 0; i < sz; i++) {
        for (int j = i + 1; j < sz; j++) {
            ll prod = (ll)beads[i] * beads[j];
            if (is_sum_sq_plus_one(prod)) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }

    // Enumerate all simple cycles of length >= 3
    // For each cycle, add potency / (2 * length) to the answer
    // Use DFS from the smallest index in cycle to avoid counting duplicates

    ll total = 0;
    int max_depth = sz; // practical limit

    // For each starting vertex, find cycles where start is the minimum index
    vector<bool> visited(sz, false);

    function<void(int, int, int, ll, int)> dfs = [&](int start, int cur, int depth, ll path_sum, int prev) {
        for (int next : adj[cur]) {
            if (next < start) continue; // ensure start is minimum
            if (next == start && depth >= 3) {
                // Found a cycle of length depth
                // Each undirected cycle is found twice (two directions),
                // but since we fix start as minimum, we find each cycle exactly twice
                // (clockwise and counterclockwise)
                // We need to divide by 2*depth for bracelets,
                // but we find each directed cycle once with start fixed as min,
                // so we find 2 directed versions -> divide by 2 to get 1 bracelet contribution
                // Then the bracelet potency is path_sum
                total += path_sum;
                continue;
            }
            if (visited[next] || next == start) continue;
            visited[next] = true;
            dfs(start, next, depth + 1, path_sum + beads[next], cur);
            visited[next] = false;
        }
    };

    for (int s = 0; s < sz; s++) {
        visited[s] = true;
        dfs(s, s, 1, beads[s], -1);
        visited[s] = false;
    }

    // total counts each bracelet exactly twice (two traversal directions with fixed min start)
    // Divide by 2
    total /= 2;

    cout << total << endl;

    return 0;
}
