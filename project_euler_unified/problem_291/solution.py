"""
Problem 291: Panaitopol Primes

Find how many primes p < 5*10^15 can be written as n^2 + n + 1
for some positive integer n.
"""

from sympy import isprime
import math

def solve():
    LIMIT = 5 * 10**15
    nmax = int(math.isqrt(LIMIT))
    # Adjust so n^2 + n + 1 < LIMIT
    while nmax * nmax + nmax + 1 >= LIMIT:
        nmax -= 1

    count = 0
    for n in range(1, nmax + 1):
        p = n * n + n + 1
        if p >= LIMIT:
            break
        if isprime(p):
            count += 1

    print(count)

if __name__ == "__main__":
    solve()
