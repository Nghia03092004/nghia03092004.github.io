#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

const ll LIMIT = 100000000000LL; // 10^11

// We need:
// r_2(n^2) = 420 => product(2*a_i+1) = 105 = 3*5*7
// Exponent patterns (sorted descending for canonical form):
// {52}, {17,1}, {10,2}, {7,3}, {3,2,1}
//
// For {52}: p^52 with p=5 gives 5^52 >> 10^11. No solutions.
// For {17,1}: smallest is 5^17 * 13 = 762939453125*13 >> 10^11.
//   Or 5^1 * 13^17 >> 10^11. No solutions.
// For {10,2}: 5^10 * 13^2 = 9765625*169 = 1,650,390,625. OK.
//   5^2 * 13^10 = 25 * 137,858,491,849 >> 10^11. Only (big exp on small prime) works.
// For {7,3}: 5^7 * 13^3 = 78125*2197 = 171,656,250. OK.
//   5^3 * 13^7 = 125*62748517 = 7,843,564,625. OK.
// For {3,2,1}: many possibilities.
//
// Strategy: for each exponent assignment to primes in increasing order,
// enumerate using DFS. The prime with exp=1 can be huge, so we need a way
// to generate primes ≡ 1 mod 4 up to ~5*10^9.
//
// For the "free multiplier" k (no prime factor ≡ 1 mod 4), we need sum_no1mod4(L).
// When L is small, sieve directly. When L is large, use inclusion-exclusion
// with primes ≡ 1 mod 4 up to L.
//
// Alternative approach: generate primes ≡ 1 mod 4 using segmented sieve,
// or just sieve up to sqrt(LIMIT) ~ 316228 and for the last (largest) prime
// in the product, iterate directly.

// Sieve primes up to some limit
vector<int> small_primes;
vector<int> primes_1mod4_small; // primes ≡ 1 mod 4 up to sieve limit

void sieve(int n) {
    vector<bool> is_p(n + 1, true);
    is_p[0] = is_p[1] = false;
    for (int i = 2; (ll)i * i <= n; i++)
        if (is_p[i])
            for (int j = i * i; j <= n; j += i)
                is_p[j] = false;
    for (int i = 2; i <= n; i++)
        if (is_p[i]) {
            small_primes.push_back(i);
            if (i % 4 == 1) primes_1mod4_small.push_back(i);
        }
}

// Check if a number > sieve_limit is prime using trial division
bool is_prime_check(ll n) {
    if (n < 2) return false;
    if (n < (ll)small_primes.back() * small_primes.back()) {
        for (int p : small_primes) {
            if ((ll)p * p > n) return true;
            if (n % p == 0) return false;
        }
        return true;
    }
    for (int p : small_primes) {
        if ((ll)p * p > n) return true;
        if (n % p == 0) return false;
    }
    return true; // probably prime if we've checked up to sqrt
}

// Sum of all k in [1, L] that have no prime factor ≡ 1 mod 4
// For small L, sieve directly.
// primes_1mod4_for_sieve are the primes ≡ 1 mod 4 up to L.
ll sum_no_1mod4(ll L) {
    if (L <= 0) return 0;
    if (L == 1) return 1;

    // Sieve: mark numbers divisible by any prime ≡ 1 mod 4
    vector<bool> bad(L + 1, false);
    for (int p : primes_1mod4_small) {
        if (p > L) break;
        for (ll j = p; j <= L; j += p)
            bad[j] = true;
    }
    ll s = 0;
    for (ll i = 1; i <= L; i++)
        if (!bad[i]) s += i;
    return s;
}

ll total_sum = 0;

// Enumerate products of primes ≡ 1 mod 4 with given exponent sequence.
// exps[pos..] are exponents to assign to primes in increasing order.
// Primes must be > prev_prime.
// current_P is the product so far.
// For the last exponent, if it's 1, we can iterate over all primes ≡ 1 mod 4
// greater than prev_prime up to LIMIT/current_P using a segmented approach.

void enumerate(const vector<int>& exps, int pos, ll prev_prime, ll current_P) {
    if (pos == (int)exps.size()) {
        ll L = LIMIT / current_P;
        ll s = sum_no_1mod4(L);
        total_sum += s * current_P;
        return;
    }

    int e = exps[pos];
    bool is_last = (pos == (int)exps.size() - 1);

    if (is_last && e == 1) {
        // Last exponent is 1: the prime p contributes p^1 to the product.
        // We need current_P * p <= LIMIT, so p <= LIMIT / current_P.
        // p must be prime, ≡ 1 mod 4, and > prev_prime.
        // For each such p, the multiplier k has L = LIMIT / (current_P * p).
        // We need to sum over all such p: sum_no_1mod4(L) * current_P * p.
        //
        // For primes within our sieve range, iterate directly.
        // For primes beyond sieve range, we'd need segmented sieve.
        // Let's use segmented sieve approach.

        ll max_p = LIMIT / current_P;

        // Use primes from our small list first
        for (int pp : primes_1mod4_small) {
            ll p = pp;
            if (p <= prev_prime) continue;
            if (p > max_p) break;
            ll new_P = current_P * p;
            ll L = LIMIT / new_P;
            ll s = sum_no_1mod4(L);
            total_sum += s * new_P;
        }

        // Now handle primes beyond our sieve limit
        ll sieve_lim = primes_1mod4_small.empty() ? 2 : primes_1mod4_small.back();
        if (max_p > sieve_lim) {
            ll seg_start = max(sieve_lim + 1, prev_prime + 1);
            // Round up to make seg_start odd if needed
            // Use segmented sieve
            const ll SEG_SIZE = 1000000;
            for (ll lo = seg_start; lo <= max_p; lo += SEG_SIZE) {
                ll hi = min(lo + SEG_SIZE - 1, max_p);
                ll sz = hi - lo + 1;
                vector<bool> is_p(sz, true);
                for (int sp : small_primes) {
                    if ((ll)sp * sp > hi) break;
                    ll start = ((lo + sp - 1) / sp) * sp;
                    if (start == sp) start += sp; // don't mark the prime itself...
                    // Actually for segmented sieve, lo > sieve_limit > sp, so start > sp always
                    for (ll j = start; j <= hi; j += sp)
                        is_p[j - lo] = false;
                }
                for (ll i = 0; i < sz; i++) {
                    if (is_p[i]) {
                        ll p = lo + i;
                        if (p <= prev_prime) continue;
                        if (p % 4 != 1) continue;
                        ll new_P = current_P * p;
                        ll L = LIMIT / new_P;
                        ll s = sum_no_1mod4(L);
                        total_sum += s * new_P;
                    }
                }
            }
        }
        return;
    }

    // General case: pick a prime ≡ 1 mod 4, raise to power e
    for (int pp : primes_1mod4_small) {
        ll p = pp;
        if (p <= prev_prime) continue;
        ll pe = 1;
        bool overflow = false;
        for (int j = 0; j < e; j++) {
            if (pe > LIMIT / p) { overflow = true; break; }
            pe *= p;
        }
        if (overflow) break;
        if (current_P > LIMIT / pe) break;
        enumerate(exps, pos + 1, p, current_P * pe);
    }

    // Also check primes beyond sieve for small exponents (e >= 2)
    // p^e <= LIMIT / current_P => p <= (LIMIT/current_P)^(1/e)
    // For e >= 2 and LIMIT = 10^11, p <= 10^(11/2) = ~316227 which is within sieve range.
    // For e = 1 handled above. For e >= 2, sieve up to 316228 is sufficient.
    // Actually for e=2: p <= sqrt(10^11 / current_P). current_P >= 1, so p <= 316227. Covered.
    // For e=3: p <= (10^11)^(1/3) ~ 46415. Covered.
    // So we're fine.
}

int main() {
    // Sieve up to enough to cover:
    // - primes for enumeration: for e=2, up to 316228
    // - primes for sum_no_1mod4: L can be up to ~LIMIT/P_min
    //   For pattern (3,2,1) with smallest P: 5^3*13^2*17 = 358525, L ~ 278854
    //   For (7,3): 5^7*13^3 = 171656250, L ~ 583
    //   For (10,2): 5^10*13^2 = 1650390625, L ~ 60
    //   For last-prime-with-exp-1 cases: L = LIMIT/(current_P * p),
    //     when p is large, L is small.
    //   Maximum L occurs for the (3,2,1) pattern with the last prime = 17 (smallest):
    //     L = 10^11 / (5^3*13^2*17) ≈ 278854. Need primes ≡ 1 mod 4 up to 278854.
    //   But wait, for the last-prime-with-exp-1 optimization, L can be up to
    //     LIMIT / (current_P * (prev_prime+1)). E.g., current_P = 5^3 * 13^2 = 21125,
    //     prev_prime = 13, smallest valid p = 17: L = 10^11 / (21125*17) ≈ 278,416.
    //   So we need sieve up to ~280,000 for sum_no_1mod4 sieving purposes,
    //   PLUS we need segmented sieve for finding large primes.
    //   For trial division in is_prime_check, we need primes up to sqrt(5*10^9) ~ 70711.

    sieve(320000); // covers all needs

    // Exponent patterns: assignments of exponents to primes in increasing order
    // For unordered multiset of exponents, all orderings give different prime assignments.
    // Patterns from 105 = 3*5*7:
    // Unordered exponent sets and their permutations:
    // {52} -> only (52)
    // {17,1} -> (17,1), (1,17)
    // {10,2} -> (10,2), (2,10)
    // {7,3} -> (7,3), (3,7)
    // {3,2,1} -> all 6 permutations

    vector<vector<int>> patterns = {
        {52},
        {17, 1}, {1, 17},
        {10, 2}, {2, 10},
        {7, 3}, {3, 7},
        {3, 2, 1}, {3, 1, 2}, {2, 3, 1}, {2, 1, 3}, {1, 3, 2}, {1, 2, 3}
    };

    total_sum = 0;
    for (auto& exps : patterns) {
        enumerate(exps, 0, 0, 1);
    }

    cout << total_sum << endl;
    return 0;
}
