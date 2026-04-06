"""
Problem 49: Prime Permutations

Find the other 3-term arithmetic sequence of 4-digit primes that are
permutations of each other, and concatenate the three terms.
"""

from collections import defaultdict

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime

def solve():
    is_prime = sieve(10000)

    groups = defaultdict(list)
    for p in range(1000, 10000):
        if is_prime[p]:
            groups[''.join(sorted(str(p)))].append(p)

    for primes in groups.values():
        if len(primes) < 3:
            continue
        pset = set(primes)
        for i in range(len(primes)):
            for j in range(i + 1, len(primes)):
                third = 2 * primes[j] - primes[i]
                if third in pset and primes[i] != 1487:
                    return f"{primes[i]}{primes[j]}{third}"

print(solve())
