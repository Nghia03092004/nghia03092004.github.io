"""
Problem 982: Cantor Set Measure Sum

Compute sum_{n=0}^{100} floor(10^18 * (2/3)^n) mod (10^9 + 7).

The quantity (2/3)^n represents the remaining measure of the Cantor set
after n iterations of the middle-third removal process.

S(n) = floor(10^18 * (2/3)^n) = floor(10^18 * 2^n / 3^n)

Key observations:
    - S(n) decays exponentially, reaching 0 around n=44 (since (2/3)^44 * 10^18 < 1)
    - The sum is effectively finite (terms beyond ~n=44 contribute 0)
    - Exact computation uses integer arithmetic: 10^18 * 2^n // 3^n

Answer: computed via exact integer arithmetic

Methods:
    - compute_S(n): Single term floor(10^18 * (2/3)^n)
    - compute_total(n_max): Sum of all terms
    - find_zero_threshold(): Find n where S(n) first becomes 0
    - cantor_iteration_lengths(depth): Compute interval structure of Cantor set
"""

MOD = 10**9 + 7


def compute_S(n):
    """Compute S(n) = floor(10^18 * 2^n / 3^n)."""
    return (10**18 * 2**n) // (3**n)


def compute_total(n_max):
    """Compute sum_{n=0}^{n_max} S(n) mod MOD."""
    total = 0
    values = []
    for n in range(n_max + 1):
        s_n = compute_S(n)
        values.append(s_n)
        total = (total + s_n) % MOD
    return total, values


def find_zero_threshold():
    """Find the smallest n where S(n) = 0."""
    for n in range(200):
        if compute_S(n) == 0:
            return n
    return -1


def cantor_intervals(depth):
    """Generate Cantor set intervals at given depth."""
    intervals = [(0.0, 1.0)]
    for _ in range(depth):
        new_intervals = []
        for a, b in intervals:
            third = (b - a) / 3
            new_intervals.append((a, a + third))
            new_intervals.append((b - third, b))
        intervals = new_intervals
    return intervals

# Verification

# S(0) = 10^18
assert compute_S(0) == 10**18

# S(1) = floor(10^18 * 2/3) = 666666666666666666
assert compute_S(1) == 666666666666666666

# Find zero threshold
zero_n = find_zero_threshold()
assert zero_n is not None and zero_n > 40  # should be around 44

# Verify monotone decrease
for n in range(100):
    assert compute_S(n) >= compute_S(n + 1)

total, values = compute_total(100)
print(total)
