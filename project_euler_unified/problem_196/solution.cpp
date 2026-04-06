#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Problem 196: Prime Triplets
//
// Triangle: row n has values T(n)+0, T(n)+1, ..., T(n)+n-1 where T(n) = n*(n-1)/2 + 1.
//
// Neighbors of (n, k) [0-indexed]:
//   Same row: (n, k-1), (n, k+1)
//   Row above (n-1): (n-1, k-1), (n-1, k), (n-1, k+1) [if valid]
//   Row below (n+1): (n+1, k-1), (n+1, k), (n+1, k+1) [if valid]
//   Up to 8 neighbors total.
//
// Prime triplet: 3 primes where ONE has the other TWO as neighbors.
// NOT a clique -- a star/path with the center having 2+ prime neighbors.
//
// A prime p in row n is in a triplet iff:
//   (a) p has >= 2 prime neighbors, OR
//   (b) p has a prime neighbor q that has >= 2 prime neighbors (q is center, p is spoke)
//
// Algorithm:
// 1. Sieve primes in rows n-3 to n+3 (need 3-row buffer to check neighbors of neighbors)
// 2. Count prime neighbors for each prime in rows n-2 to n+2
// 3. Mark primes in row n that satisfy (a) or (b)

ll row_start(ll n) {
    return n * (n - 1) / 2 + 1;
}

vector<char> segmented_sieve(ll lo, ll hi) {
    ll range = hi - lo + 1;
    vector<char> is_prime(range, 1);
    if (lo <= 0) {
        for (ll i = 0; i < min(range, 1 - lo + 1); i++)
            is_prime[i] = 0;
    }
    if (lo <= 1 && 1 <= hi) is_prime[1 - lo] = 0;

    ll sqrt_hi = (ll)sqrt((double)hi) + 2;
    vector<char> small(sqrt_hi + 1, 1);
    small[0] = small[1] = 0;
    for (ll i = 2; i <= sqrt_hi; i++) {
        if (small[i]) {
            for (ll j = i * i; j <= sqrt_hi; j += i)
                small[j] = 0;
            ll start = max(i * i, ((lo + i - 1) / i) * i);
            for (ll j = start; j <= hi; j += i)
                is_prime[j - lo] = 0;
        }
    }
    return is_prime;
}

ll solve(ll n) {
    // Need rows n-3 to n+3 for the full neighbor-of-neighbor check
    ll r_min = max(1LL, n - 3);
    ll r_max = n + 3;
    ll lo = row_start(r_min);
    ll hi = row_start(r_max) + r_max - 1;

    auto sieve = segmented_sieve(lo, hi);

    auto prime = [&](ll v) -> bool {
        if (v < lo || v > hi) return false;
        return sieve[v - lo];
    };

    // For each prime in rows n-2..n+2, count how many prime neighbors it has.
    // A prime with >= 2 prime neighbors is a "center" of at least one triplet.
    // Store: for each cell in rows n-2..n+2, the count of prime neighbors.

    // We'll work row by row. For efficiency, store prime neighbor counts.
    // But we only need to know: for each prime in rows n-1, n, n+1,
    // does it have >= 2 prime neighbors?
    // And for primes in row n: also check if any neighbor has >= 2 prime neighbors.

    // Step 1: For each prime in rows n-2..n+2, count prime neighbors
    // (neighbors span rows r-1..r+1, so we need rows n-3..n+3 in the sieve)

    // For memory efficiency: store "has_2_prime_nbs" flag for primes in rows n-2..n+2
    // Then for row n: check condition (a) or (b).

    // Actually let's just compute for rows n-1, n, n+1 whether each prime has >= 2 prime nbs.
    // For condition (b), a prime p in row n needs a neighbor q in rows n-1 or n+1 (or n)
    // that has >= 2 prime neighbors. q's neighbors span rows n-2..n+2.

    // So we need "has_2_prime_nbs" for primes in rows n-1, n, n+1.
    // To compute that, we need prime status in rows n-2..n+2.
    // The sieve covers rows n-3..n+3, which is sufficient.

    // Build arrays of "has >= 2 prime neighbors" for rows n-1, n, n+1
    auto count_prime_nbs = [&](ll row, ll pos) -> int {
        int cnt = 0;
        ll rs = row_start(row);
        ll v = rs + pos;
        // Same row
        if (pos > 0 && prime(v - 1)) cnt++;
        if (pos < row - 1 && prime(v + 1)) cnt++;
        // Row above
        if (row > 1) {
            ll above = row_start(row - 1);
            for (int dk = -1; dk <= 1; dk++) {
                ll p = pos + dk;
                if (p >= 0 && p < row - 1) {
                    if (prime(above + p)) cnt++;
                }
            }
        }
        // Row below
        {
            ll below = row_start(row + 1);
            for (int dk = -1; dk <= 1; dk++) {
                ll p = pos + dk;
                if (p >= 0 && p < row + 1) {
                    if (prime(below + p)) cnt++;
                }
            }
        }
        return cnt;
    };

    // For each prime in row n, check:
    // (a) it has >= 2 prime neighbors
    // (b) it has a prime neighbor (in rows n-1, n, n+1) that has >= 2 prime neighbors

    ll n_start = row_start(n);
    ll result = 0;

    for (ll k = 0; k < n; k++) {
        ll v = n_start + k;
        if (!prime(v)) continue;

        int my_cnt = count_prime_nbs(n, k);
        if (my_cnt >= 2) {
            result += v;
            continue;
        }

        // Check condition (b): does any prime neighbor have >= 2 prime neighbors?
        bool found = false;

        // Same row neighbors
        if (k > 0 && prime(v - 1)) {
            if (count_prime_nbs(n, k - 1) >= 2) { found = true; }
        }
        if (!found && k < n - 1 && prime(v + 1)) {
            if (count_prime_nbs(n, k + 1) >= 2) { found = true; }
        }

        // Row above neighbors
        if (!found && n > 1) {
            ll above = row_start(n - 1);
            for (int dk = -1; dk <= 1 && !found; dk++) {
                ll p = k + dk;
                if (p >= 0 && p < n - 1 && prime(above + p)) {
                    if (count_prime_nbs(n - 1, p) >= 2) { found = true; }
                }
            }
        }

        // Row below neighbors
        if (!found) {
            ll below = row_start(n + 1);
            for (int dk = -1; dk <= 1 && !found; dk++) {
                ll p = k + dk;
                if (p >= 0 && p < n + 1 && prime(below + p)) {
                    if (count_prime_nbs(n + 1, p) >= 2) { found = true; }
                }
            }
        }

        if (found) result += v;
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    ll ans = solve(5678027) + solve(7208785);
    cout << ans << endl;
    return 0;
}
