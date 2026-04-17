"""
Project Euler Problem 241: Perfection Quotients

Find the sum of all n <= 10^18 such that sigma(n)/n is a half-integer,
i.e., 2*sigma(n)/n is an odd integer >= 3.

We perform a backtracking search over the prime factorisation of n,
maintaining 2*sigma(n)/n as a reduced fraction num/den.
"""

from math import gcd
from sympy import isprime

LIMIT = 10**18
results = []


def sigma_pe(p, e):
    """Return sigma(p^e) = 1 + p + ... + p^e."""
    return (p**(e + 1) - 1) // (p - 1)


def search(n_val, num, den, min_prime):
    """
    n_val: current partial product
    num/den: 2*sigma(n_val)/n_val in lowest terms
    min_prime: smallest prime allowed as next factor
    """
    if den == 1 and num % 2 == 1 and num >= 3 and n_val > 1:
        results.append(n_val)

    if num > 200 * den:
        return

    max_p = LIMIT // n_val
    if max_p < min_prime:
        return

    candidates = set()

    for p in small_primes:
        if p > max_p:
            break
        if p >= min_prime:
            candidates.add(p)

    if den > 1:
        divisors_of_den = []
        d = 1
        while d * d <= den:
            if den % d == 0:
                divisors_of_den.append(d)
                if d != den // d:
                    divisors_of_den.append(den // d)
            d += 1
        for d in divisors_of_den:
            for c in [d - 1, d + 1, 2 * d - 1, 2 * d + 1]:
                if min_prime <= c <= max_p and c > 1 and isprime(c):
                    candidates.add(c)

    for p in sorted(candidates):
        pe = 1
        sig = 1
        for e in range(1, 200):
            if pe > max_p // p:
                break
            pe *= p
            sig += pe

            new_n = n_val * pe
            if new_n > LIMIT:
                break

            g1 = gcd(num, pe)
            g2 = gcd(sig, den)
            new_num = (num // g1) * (sig // g2)
            new_den = (den // g2) * (pe // g1)
            g3 = gcd(new_num, new_den)
            new_num //= g3
            new_den //= g3

            search(new_n, new_num, new_den, p + 1)


def sieve(limit):
    is_p = [True] * (limit + 1)
    is_p[0] = is_p[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_p[i]:
            for j in range(i * i, limit + 1, i):
                is_p[j] = False
    return [i for i in range(2, limit + 1) if is_p[i]]


small_primes = sieve(2_000_000)

search(1, 2, 1, 2)

print(sum(results))
