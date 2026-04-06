#!/usr/bin/env python3
"""
Problem 278: Linear Combinations of Semiprimes

For distinct primes p, q, r, compute f(p,q,r) = 2pqr - pq - qr - rp.
Sum f over all unordered triples {p,q,r} of distinct primes up to 5000.

Uses prefix sums for O(pi(N)^2) efficiency.
"""

def sieve(limit):
    """Return list of primes up to limit."""
    is_prime = [False, False] + [True] * (limit - 1)
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, limit + 1, i):
                is_prime[j] = False
    return [i for i in range(2, limit + 1) if is_prime[i]], is_prime

def solve():
    LIMIT = 5000
    primes, is_prime = sieve(LIMIT)

    # Prefix sums and counts
    ps = [0] * (LIMIT + 1)
    pc = [0] * (LIMIT + 1)
    for i in range(1, LIMIT + 1):
        ps[i] = ps[i-1] + (i if is_prime[i] else 0)
        pc[i] = pc[i-1] + (1 if is_prime[i] else 0)

    answer = 0
    for i, p in enumerate(primes):
        for j in range(i + 1, len(primes)):
            q = primes[j]

            # r ranges over all primes > q, <= LIMIT
            sum_r = ps[LIMIT] - ps[q]
            count_r = pc[LIMIT] - pc[q]
            if count_r == 0:
                continue

            coeff = 2 * p * q - p - q
            answer += coeff * sum_r - p * q * count_r

    return answer

if __name__ == "__main__":
    answer = solve()
    print(answer)
