"""
Problem 293: Pseudo-Fortunate Numbers

Find the sum of all distinct pseudo-Fortunate numbers for admissible numbers <= 10^9.

An admissible number uses consecutive primes starting from 2, each at least once.
The pseudo-Fortunate number for n is the smallest m > 1 such that n + m is prime.
"""

from sympy import isprime

LIMIT = 10**9
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]

def gen_admissible():
    """Generate all admissible numbers <= LIMIT."""
    result = []

    def gen(idx, val):
        if idx > 0:
            result.append(val)
        if idx >= len(primes):
            return
        p = primes[idx]
        v = val * p
        while v <= LIMIT:
            gen(idx + 1, v)
            v *= p

    gen(0, 1)
    return result

def solve():
    admissible = gen_admissible()
    pf_set = set()

    for n in admissible:
        m = 2
        while not isprime(n + m):
            m += 1
        pf_set.add(m)

    print(sum(pf_set))

if __name__ == "__main__":
    solve()
