"""
Problem 118: Pandigital Prime Sets

Find the number of distinct sets of primes that use each digit 1-9 exactly once.
"""

from itertools import permutations

def isprime(n):
    """Deterministic primality test for n up to ~10^9."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def solve():
    # Precompute: for each bitmask of digits {1..9}, find all primes
    mask_primes = {}

    for mask in range(1, 512):
        digits = []
        for i in range(9):
            if mask & (1 << i):
                digits.append(i + 1)

        primes = set()
        for perm in permutations(digits):
            num = int(''.join(map(str, perm)))
            if isprime(num):
                primes.add(num)

        if primes:
            mask_primes[mask] = sorted(primes)

    # DFS: partition all 9 digits into sets forming primes in increasing order
    count = 0

    def dfs(remaining, last_prime):
        nonlocal count
        if remaining == 0:
            count += 1
            return

        # Enumerate non-empty subsets of remaining
        sub = remaining
        while sub > 0:
            if sub in mask_primes:
                for p in mask_primes[sub]:
                    if p > last_prime:
                        dfs(remaining ^ sub, p)
            sub = (sub - 1) & remaining

    dfs(511, 0)
    return count

answer = solve()
assert answer == 44680
print(answer)
