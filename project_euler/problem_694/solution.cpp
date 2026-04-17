#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 694: Cube-full Divisors
 *
 * s(n) = number of cube-full divisors of n (where p|d => p^3|d for all primes p).
 * S(n) = sum_{i=1}^{n} s(i).
 * Find S(10^18).
 *
 * Key insight: S(n) = sum over all cube-full numbers d of floor(n/d).
 * A cube-full number has the form: product of p_i^{a_i} where all a_i >= 3.
 * We can write any cube-full number as a^3 * b where b is square-free and
 * gcd(a, b) ... no, that's not right.
 *
 * Actually a cube-full number c can be written as c = a^3 * b^4 * d^5 ...
 * More precisely, enumerate cube-full numbers by their prime factorization.
 *
 * Approach: enumerate all cube-full numbers up to N = 10^18.
 * A cube-full number c = p1^a1 * p2^a2 * ... where all ai >= 3.
 * We can write c = m^3 * k where k is also cube-full, but this overcounts.
 *
 * Better: recursively enumerate cube-full numbers.
 * For each prime p, try exponents 3, 4, 5, 6, ..., and recurse.
 * Since p^3 <= N, we need primes up to N^(1/3) = 10^6.
 *
 * S(N) = sum over all cube-full c <= N of floor(N / c).
 */

typedef long long ll;
typedef unsigned long long ull;

const ll N = 1000000000000000000LL; // 10^18

// Sieve primes up to 10^6
vector<int> primes;
vector<bool> is_prime;

void sieve(int limit) {
    is_prime.assign(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
}

ll answer = 0;

// Recursively enumerate cube-full numbers.
// cur = current cube-full number built so far
// pidx = index of next prime to consider (to avoid permutations)
// We try each prime p = primes[pidx] with exponent e >= 3,
// as long as cur * p^3 <= N.
void enumerate(ll cur, int pidx) {
    // cur is a cube-full number; add floor(N / cur) to answer.
    answer += N / cur;

    // Try next primes
    for (int i = pidx; i < (int)primes.size(); i++) {
        ll p = primes[i];
        ll p3 = p * p * p;
        if (cur > N / p3) break; // cur * p^3 > N

        ll val = cur * p3;
        for (int e = 3; val <= N; e++) {
            enumerate(val, i + 1);
            if (val > N / p) break;
            val *= p;
        }
    }
}

int main() {
    // N^(1/3) = 10^6
    sieve(1000001);

    enumerate(1, 0);

    cout << answer << endl;
    return 0;
}
