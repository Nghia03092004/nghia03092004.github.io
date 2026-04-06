"""
Problem 986: Partition Rank Statistics — R(n) = 0 by Conjugation

For a partition lambda of n, define rank(lambda) = largest_part - number_of_parts.
Prove/verify that R(n) = sum of rank(lambda) over all partitions of n equals 0
for every positive integer n. This follows from the involution of conjugation:
conjugating a partition swaps largest_part and number_of_parts, negating the rank.

Key results:
    - R(n) = 0 for all n >= 1 (proven by conjugation symmetry)
    - The rank distribution is symmetric about 0
    - Number of partitions p(n) grows exponentially
    - Rank variance grows roughly linearly with n

Methods:
    1. partitions           — generate all partitions of n recursively
    2. conjugate            — compute the conjugate (transpose) of a partition
    3. rank_distribution    — compute full rank histogram for partitions of n
    4. rank_variance        — measure the spread of rank values across n
"""

from collections import Counter

def partitions(n, max_part=None):
    """Generate all partitions of n with parts <= max_part."""
    if max_part is None:
        max_part = n
    if n == 0:
        return [()]
    result = []
    for p in range(min(n, max_part), 0, -1):
        for rest in partitions(n - p, p):
            result.append((p,) + rest)
    return result

def conjugate(partition):
    """Return the conjugate partition (transpose of Young diagram)."""
    if not partition:
        return ()
    conj = []
    for i in range(1, partition[0] + 1):
        conj.append(sum(1 for p in partition if p >= i))
    return tuple(conj)

def rank_distribution(n):
    """Compute rank = largest_part - num_parts for all partitions of n."""
    parts = partitions(n)
    ranks = [p[0] - len(p) for p in parts]
    return ranks

def rank_variance(n):
    """Compute variance of rank over all partitions of n."""
    ranks = rank_distribution(n)
    if not ranks:
        return 0.0
    mean = sum(ranks) / len(ranks)
    return sum((r - mean) ** 2 for r in ranks) / len(ranks)

# Verification with known values
# Verify R(n) = 0 for n = 1 through 25
for n in range(1, 26):
    total_rank = sum(p[0] - len(p) for p in partitions(n))
    assert total_rank == 0, f"R({n}) = {total_rank} != 0"

# Verify conjugation negates rank for n = 10
for p in partitions(10):
    c = conjugate(p)
    rank_p = p[0] - len(p)
    rank_c = c[0] - len(c)
    assert rank_p == -rank_c, f"Conjugation rank mismatch: {p} vs {c}"

# Verify partition counts p(n) for known values
assert len(partitions(1)) == 1
assert len(partitions(5)) == 7
assert len(partitions(10)) == 42

# Compute answer
answer = 0
print(answer)
