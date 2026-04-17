"""
Problem 958: Subset Sum Counting

Count subsets of {2^0, 2^1, ..., 2^19} that sum to 500000.

Since powers of 2 form a unique binary representation system, any integer
in [0, 2^20 - 1] has exactly one subset of {2^0,...,2^19} that sums to it.
500000 < 2^19 = 524288? No, 500000 < 2^20 = 1048576 and 500000 in binary
is 1111010000100100000, which uses bits within 0..19.
Answer: 1.

Key results:
    - 500000 = 0b1111010000100100000, uses bits {5, 8, 14, 15, 16, 17, 18}
    - Unique binary representation => exactly 1 subset
    - DP verification confirms the answer

Methods:
    1. binary_representation   — direct binary decomposition
    2. count_subset_sums_dp    — dynamic programming verification
    3. exhaustive_small_verify — brute-force for small sets
    4. subset_sum_distribution — DP-based distribution for analysis
"""

from math import comb

def binary_representation(target, max_power):
    """Check if target can be represented using powers 2^0 .. 2^max_power."""
    bits_used = []
    for i in range(max_power, -1, -1):
        if target >= 2 ** i:
            target -= 2 ** i
            bits_used.append(i)
    if target == 0:
        return bits_used
    return None

def count_subset_sums_dp(elements, target):
    """Count number of subsets of elements summing to target."""
    dp = [0] * (target + 1)
    dp[0] = 1
    for e in elements:
        for t in range(target, e - 1, -1):
            dp[t] += dp[t - e]
    return dp[target]

def exhaustive_subset_sums(elements):
    """Return dict: target -> count of subsets summing to it."""
    from collections import defaultdict
    counts = defaultdict(int)
    n = len(elements)
    for mask in range(2 ** n):
        s = sum(elements[i] for i in range(n) if mask & (1 << i))
        counts[s] += 1
    return counts

def subset_sum_distribution(elements):
    """Return full DP array of subset sum counts."""
    max_sum = sum(elements)
    dp = [0] * (max_sum + 1)
    dp[0] = 1
    for e in elements:
        for t in range(max_sum, e - 1, -1):
            dp[t] += dp[t - e]
    return dp

# Verification
# Powers of 2 have unique binary representation: each target 0..2^n-1 has exactly 1 subset
small_elements = [2 ** i for i in range(8)]
small_counts = exhaustive_subset_sums(small_elements)
for t in range(256):
    assert small_counts[t] == 1, f"Expected 1 subset for {t}, got {small_counts[t]}"

# Verify with DP for small case
for t in [0, 1, 3, 7, 15, 31, 63, 127, 255]:
    assert count_subset_sums_dp(small_elements, t) == 1

# Verify 500000 is representable with bits 0..19
bits_used = binary_representation(500000, 19)
assert bits_used is not None, "500000 cannot be represented with 2^0..2^19"

# Compute answer
target = 500000
elements = [2 ** i for i in range(20)]
answer = count_subset_sums_dp(elements, target)
print(f"500000 in binary: {bin(500000)}")
print(f"Bits used: {bits_used}")
print(answer)
