#!/usr/bin/env python3
"""Project Euler Problem 571: Super Pandigital Numbers

Find the sum of the 10 smallest 12-super-pandigital numbers.
"""

from itertools import permutations


def is_pandigital(n, base):
    """Return True iff n is pandigital in the given base (bitmask test)."""
    mask = 0
    target = (1 << base) - 1
    while n > 0:
        mask |= 1 << (n % base)
        n //= base
        if mask == target:
            return True
    return mask == target


def solve():
    NEED = 10
    pow12 = [12 ** i for i in range(12)]
    results = []

    for first in range(1, 12):
        rest = [d for d in range(12) if d != first]
        for perm in permutations(rest):
            n = first * pow12[11]
            for i, d in enumerate(perm):
                n += d * pow12[10 - i]

            if all(is_pandigital(n, b) for b in range(11, 1, -1)):
                results.append(n)

        if first == 1 and len(results) >= NEED:
            break

    results.sort()
    print(sum(results[:NEED]))


if __name__ == "__main__":
    solve()
