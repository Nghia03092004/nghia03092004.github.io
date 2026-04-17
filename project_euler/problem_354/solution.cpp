#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 354: Distances in a Bee's Honeycomb
 *
 * Find count of n <= 10^12 with C(n) = 450 where C(n) = 6*(d1(n) - d2(n))
 * and d1, d2 count divisors congruent to 1, 2 mod 3 respectively.
 *
 * We need d1(n) - d2(n) = 75, with all primes == 2 mod 3 having even exponents.
 * Product of (a_i + 1) for primes == 1 mod 3 must equal 75.
 *
 * Factorizations of 75: {75}, {25,3}, {15,5}, {5,5,3}
 * Exponent patterns: (74), (24,2), (14,4), (4,4,2)
 *
 * We enumerate using backtracking over primes.
 */

typedef long long ll;
const ll LIMIT = 1000000000000LL; // 10^12

// Primes == 1 mod 3 (up to a reasonable bound)
vector<ll> primes1; // primes p with p == 1 mod 3
vector<ll> primes2; // primes p with p == 2 mod 3

void sieve_primes(ll bound) {
    vector<bool> is_prime(bound + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (ll i = 2; i <= bound; i++) {
        if (!is_prime[i]) continue;
        for (ll j = i * i; j <= bound; j += i)
            is_prime[j] = false;
        if (i == 3) continue; // skip 3 itself
        if (i % 3 == 1) primes1.push_back(i);
        else if (i % 3 == 2) primes2.push_back(i);
    }
}

ll answer = 0;

// Count numbers <= limit that are products of even powers of primes == 2 mod 3
// (i.e., squares of squarefree numbers composed of primes == 2 mod 3) times powers of 3
// Actually: products of 3^a * product(q_j^{2*b_j}) for any a >= 0, b_j >= 1 or 0
// We need to count n <= limit of the form 3^a * m^2 where m is composed only of primes == 2 mod 3
// Wait -- not exactly. We need n such that n = base * k where base is the product of primes == 1 mod 3
// part, and k = 3^a * product(q_j^{2*c_j}) for any a >= 0, c_j >= 0.
// k can be any number whose odd part (removing factor 3) is a perfect square of a 2-mod-3-smooth number.

// Count of k <= L where k = 3^a * s^2, s composed of primes == 2 mod 3, a >= 0
// = sum_{a=0}^{...} count of s^2 <= L/3^a where s is product of primes == 2 mod 3
// This is complex. Let's use a simpler recursive enumeration.

// Count square numbers (including 1) up to L composed only of primes == 2 mod 3, times any power of 3
void count_multipliers(ll limit, int idx2, ll current, ll& cnt) {
    // current = product so far of (primes2 squares and powers of 3)
    // Add all powers of 3 multiplied by current
    ll val = current;
    while (val <= limit) {
        cnt++;
        val *= 3;
    }

    // Try adding q^2 for primes2[idx2], primes2[idx2+1], ...
    for (int i = idx2; i < (int)primes2.size(); i++) {
        ll q = primes2[i];
        ll q2 = q * q;
        if (current > limit / q2) break; // overflow check
        ll next = current * q2;
        while (next <= limit) {
            count_multipliers(limit, i + 1, next, cnt);
            if (next > limit / q2) break;
            next *= q2;
        }
    }
}

// Enumerate products of primes == 1 mod 3 with given exponent pattern
// exps is sorted decreasingly
void enumerate(vector<int>& exps, int idx, int p1_idx, ll current) {
    if (idx == (int)exps.size()) {
        // current is the "base" from primes == 1 mod 3
        // Now count multipliers: numbers m such that current * m <= LIMIT
        // and m = 3^a * (square from primes == 2 mod 3)
        ll rem = LIMIT / current;
        ll cnt = 0;
        count_multipliers(rem, 0, 1, cnt);
        answer += cnt;
        return;
    }

    int e = exps[idx];
    // Choose a prime from primes1[p1_idx..]
    int start = p1_idx;
    // If same exponent as previous, start from same index+1 to avoid duplicates
    // (handled by requiring increasing prime indices for same exponents)

    for (int i = start; i < (int)primes1.size(); i++) {
        ll p = primes1[i];
        ll pe = 1;
        bool overflow = false;
        for (int j = 0; j < e; j++) {
            if (pe > LIMIT / p) { overflow = true; break; }
            pe *= p;
        }
        if (overflow || pe > LIMIT) break;
        if (current > LIMIT / pe) break;

        ll next = current * pe;
        // For next exponent, if same value, start from i+1
        int next_start = (idx + 1 < (int)exps.size() && exps[idx+1] == e) ? i + 1 : 0;
        enumerate(exps, idx + 1, next_start, next);
    }
}

int main() {
    // Sieve primes up to sqrt(10^12) ~ 10^6
    sieve_primes(1000000);

    // Exponent patterns for product = 75
    // 75 = 75, 25*3, 15*5, 5*5*3
    vector<vector<int>> patterns = {
        {74},
        {24, 2},
        {14, 4},
        {4, 4, 2}
    };

    for (auto& pat : patterns) {
        enumerate(pat, 0, 0, 1);
    }

    cout << answer << endl;

    return 0;
}
