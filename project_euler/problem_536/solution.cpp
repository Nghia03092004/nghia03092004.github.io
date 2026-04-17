#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 536: Modulo Power Identity
 *
 * Find S(10^12) where S(n) = sum of all m <= n such that
 * a^(m+4) ≡ a (mod m) for all integers a.
 *
 * m must be squarefree, and for each prime p | m: (p-1) | (m+3).
 *
 * DFS approach: build m by adding primes.
 * When we have current product M and want to add prime p (getting M' = M*p):
 *   - For p itself: (p-1) | (M*p + 3). Since M*p ≡ M (mod p-1), need (p-1) | (M+3).
 *   - For existing prime q | M: (q-1) | (M*p + 3).
 *     Previously we may have had (q-1) | (M_old + 3) at some earlier step, but
 *     now we need (q-1) | (M*p + 3).
 *
 * Strategy: track the set of prime factors, and for each candidate new prime p,
 * verify ALL conditions for the new product M*p.
 */

typedef long long ll;
typedef __int128 lll;

const ll LIMIT = 1000000000000LL; // 10^12

ll answer = 0;

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

vector<ll> get_divisors(ll n) {
    vector<ll> divs;
    for (ll d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            divs.push_back(d);
            if (d != n / d) divs.push_back(n / d);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

// primes: sorted list of prime factors of current m
// m: current product
void dfs(ll m, vector<ll>& primes) {
    // Check if current m is valid
    ll mp3 = m + 3;
    bool valid = true;
    for (ll p : primes) {
        if (mp3 % (p - 1) != 0) {
            valid = false;
            break;
        }
    }
    if (valid) {
        answer += m;
    }

    // Try adding a new prime p > primes.back() (or p >= 2 if primes empty)
    ll min_p = primes.empty() ? 2 : primes.back() + 1;

    // For new prime p: (p-1) | (m*p + 3), which means (p-1) | (m+3)
    // So p-1 must divide m+3
    vector<ll> divs = get_divisors(mp3);

    for (ll d : divs) {
        ll p = d + 1;
        if (p < min_p) continue;
        if (!is_prime(p)) continue;
        if ((lll)m * p > LIMIT) break; // divs are sorted, so product only grows

        ll new_m = m * p;
        // We don't check validity here - we check at the start of the recursion
        primes.push_back(p);
        dfs(new_m, primes);
        primes.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);

    vector<ll> primes;
    dfs(1, primes);

    cout << answer << endl;

    return 0;
}
