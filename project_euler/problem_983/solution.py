"""
Problem 983: Digit Permutation Primes

Count the number of "digit permutation groups" of size >= 4 among primes below 10^6.

A digit permutation group is a set of primes that are anagrams of each other
(i.e., they share the same multiset of digits). We count how many such groups
contain at least 4 primes.

Key observations:
    - Group primes by their sorted digit tuple
    - Most groups are small (size 1-2)
    - Larger groups tend to appear for numbers with more digits
    - The largest groups tend to use digits that avoid divisibility patterns

Answer: computed by sieve + grouping

Methods:
    - sieve_primes(N): Sieve of Eratosthenes up to N
    - group_by_digits(primes): Group primes by sorted digit tuple
    - count_groups_by_size(groups): Distribution of group sizes
    - largest_groups(groups, top_k): Find the largest permutation groups
"""

from math import isqrt
from collections import defaultdict, Counter


def sieve_primes(N):
    """Return a boolean sieve and list of primes up to N."""
    is_p = bytearray(b'\x01') * N
    is_p[0] = is_p[1] = 0
    for i in range(2, isqrt(N - 1) + 1):
        if is_p[i]:
            is_p[i * i::i] = bytearray(len(is_p[i * i::i]))
    primes = [p for p in range(2, N) if is_p[p]]
    return is_p, primes


def group_by_digits(primes):
    """Group primes by their sorted digit tuple."""
    groups = defaultdict(list)
    for p in primes:
        key = tuple(sorted(str(p)))
        groups[key].append(p)
    return groups


def largest_groups(groups, top_k=10):
    """Return the top_k largest digit permutation groups."""
    sorted_groups = sorted(groups.values(), key=len, reverse=True)
    return sorted_groups[:top_k]

# Verification

N = 10**6
is_p, primes = sieve_primes(N)

# Basic sieve verification
assert is_p[2] and is_p[3] and is_p[5] and is_p[7]
assert not is_p[4] and not is_p[6] and not is_p[9]

groups = group_by_digits(primes)

# Known: {1478, 1487, 4817, 4871, 7481, 7841, 8147, 8741} has 8 primes (if all prime)
# Verify small example: 13, 31 are both prime
assert 13 in groups[tuple(sorted('13'))]
assert 31 in groups[tuple(sorted('13'))]

count = sum(1 for g in groups.values() if len(g) >= 4)
print(count)
