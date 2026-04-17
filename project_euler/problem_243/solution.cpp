#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 243: Resilience
 *
 * Find the smallest d such that phi(d)/(d-1) < 15499/94744.
 *
 * Strategy:
 * 1. Compute primorials P_k = 2*3*5*...*p_k
 * 2. For each k, check if R(P_k) < target.
 * 3. Once P_k overshoots, for k-1 try d = m * P_{k-1} for m = 1, 2, ...
 *    and find the smallest d satisfying the inequality.
 *
 * For d = m * P_k where gcd(m, P_k) = g:
 *   phi(d) = phi(m) * phi(P_k) * g / phi(g)
 *
 * If m divides P_k (i.e., m is a product of primes in P_k), then:
 *   phi(m * P_k) = m * P_k * prod_{p | P_k}(1 - 1/p) = m * phi(P_k)
 *   (since m*P_k has the same prime factors as P_k when m | P_k^inf)
 *
 * Actually more precisely: if m's prime factors are all among those of P_k, then
 *   phi(m * P_k) = m * phi(P_k) (since the set of prime factors doesn't change,
 *   and phi(n) = n * prod(1 - 1/p))... wait, that's only true if m is a prime power
 *   of existing primes. In general:
 *   phi(m * P_k) = (m * P_k) * prod_{p | (m * P_k)} (1 - 1/p)
 *   If rad(m) | P_k, then the prime set is the same, so:
 *   phi(m * P_k) = m * P_k * prod_{p | P_k}(1-1/p) = m * phi(P_k).
 */

typedef long long ll;
typedef __int128 lll;

int main() {
    ios_base::sync_with_stdio(false);

    // Target: 15499/94744
    ll target_num = 15499;
    ll target_den = 94744;

    // Primes
    vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

    // Compute primorials and their totients
    // P_k, phi(P_k)
    vector<ll> primorial(1, 1), phi_primorial(1, 1);

    for (int i = 0; i < (int)primes.size(); i++) {
        int p = primes[i];
        primorial.push_back(primorial.back() * p);
        phi_primorial.push_back(phi_primorial.back() * (p - 1));
    }

    // For each k, check if R(P_k) < target, i.e., phi(P_k) * target_den < target_num * (P_k - 1)
    // Find the first k where R(P_k) < target
    int first_k = -1;
    for (int k = 1; k <= (int)primes.size(); k++) {
        ll pk = primorial[k];
        ll phik = phi_primorial[k];
        // Check phik / (pk - 1) < target_num / target_den
        // i.e., phik * target_den < target_num * (pk - 1)
        // Use __int128 to avoid overflow
        lll lhs = (lll)phik * target_den;
        lll rhs = (lll)target_num * (pk - 1);
        if (lhs < rhs) {
            first_k = k;
            break;
        }
    }

    // Now try d = m * P_{k-1} for increasing m, where m's prime factors
    // are all <= p_{k-1} (the primes in P_{k-1}).
    // Also try d = m * P_{k-2}, etc.
    // We want the globally smallest d.

    ll best_d = primorial[first_k]; // This works, but may not be smallest

    // Try for each base primorial P_j where j < first_k
    for (int j = 1; j < first_k; j++) {
        ll pj = primorial[j];
        ll phij = phi_primorial[j];

        // d = m * pj, phi(d) depends on m's factorization
        // If m's prime factors are subset of primes in P_j:
        //   phi(d) = m * phij
        //   R(d) = m * phij / (m * pj - 1) < target_num / target_den
        //   m * phij * target_den < target_num * (m * pj - 1)
        //   m * phij * target_den < target_num * m * pj - target_num
        //   m * (phij * target_den - target_num * pj) < -target_num
        //   Since phij * target_den - target_num * pj < 0 (because R(P_j) > target for j < first_k):
        //   m > target_num / (target_num * pj - phij * target_den)

        lll denom = (lll)target_num * pj - (lll)phij * target_den;
        if (denom <= 0) continue;

        // m > target_num / denom
        ll m_min = (ll)(target_num / (long long)denom) + 1;

        // But m must have all prime factors in P_j's primes.
        // The smallest such m >= m_min: could be m_min if it's smooth w.r.t. P_j's primes,
        // or we need to search.

        // Generate smooth numbers (all prime factors <= primes[j-1]) near m_min
        // Actually, m can also introduce NEW primes, but then phi(d) changes.
        // Let's handle both cases.

        // Case 1: m smooth w.r.t. P_j
        // Find smallest smooth m >= m_min
        // Use BFS/priority queue to generate smooth numbers
        {
            // For small m_min, just check each m
            if (m_min <= 1000000) {
                for (ll m = max(m_min, 2LL); m <= 1000000; m++) {
                    // Check if m is smooth w.r.t. primes[0..j-1]
                    ll t = m;
                    bool smooth = true;
                    for (int i = 0; i < j; i++) {
                        while (t % primes[i] == 0) t /= primes[i];
                    }
                    if (t != 1) smooth = false;
                    if (smooth) {
                        ll d = m * pj;
                        if (d < best_d) {
                            best_d = d;
                        }
                        break; // smallest smooth m found
                    }
                }
            }
        }

        // Case 2: m introduces exactly one new prime p (not in P_j)
        // Then d = m * pj has prime factors = P_j's primes union {p}
        // phi(d) = d * prod_{q | d}(1 - 1/q)
        //        = m * pj * prod_{q in P_j}(1-1/q) * (1-1/p)
        //        = m * phij * (1 - 1/p)
        //        = m * phij * (p-1)/p
        // R(d) = m * phij * (p-1) / (p * (m*pj - 1))
        // We want this < target_num / target_den
        // m * phij * (p-1) * target_den < target_num * p * (m * pj - 1)
        // m * (phij * (p-1) * target_den - target_num * p * pj) < -target_num * p
        // If the coefficient of m is negative:
        // m > target_num * p / (target_num * p * pj - phij * (p-1) * target_den)

        for (int pi = j; pi < (int)primes.size() && pi < j + 5; pi++) {
            int p = primes[pi];
            if (pi < j) continue; // already in P_j

            lll coeff = (lll)target_num * p * pj - (lll)phij * (p-1) * target_den;
            if (coeff <= 0) continue;

            ll m_min2 = (ll)((lll)target_num * p / coeff) + 1;

            // m must be of form p^a * (smooth w.r.t. P_j), with a >= 1
            // Smallest: m = p
            if (p >= m_min2) {
                ll d = (ll)p * pj;
                if (d < best_d) best_d = d;
            } else {
                // Try m = p * s for small smooth s
                for (ll s = 1; s <= 10000; s++) {
                    ll t = s;
                    bool smooth = true;
                    for (int i = 0; i < j; i++) {
                        while (t % primes[i] == 0) t /= primes[i];
                    }
                    if (t != 1) smooth = false;
                    if (!smooth) continue;

                    ll m = s * p;
                    if (m >= m_min2) {
                        ll d = m * pj;
                        if (d < best_d) best_d = d;
                        break;
                    }
                }
            }
        }
    }

    cout << best_d << endl;
    return 0;
}
