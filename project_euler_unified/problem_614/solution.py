"""
Problem 614: Special Partitions 2
Count integer partitions with ordering and distinctness constraints.
"""

def count_partitions(n):
    """Count partitions of n (standard)."""
    dp = [0] * (n + 1)
    dp[0] = 1
    for k in range(1, n + 1):
        for j in range(k, n + 1):
            dp[j] += dp[j - k]
    return dp[n]

def count_distinct_partitions(n):
    """Count partitions of n into distinct parts."""
    dp = [0] * (n + 1)
    dp[0] = 1
    for k in range(1, n + 1):
        for j in range(n, k - 1, -1):
            dp[j] += dp[j - k]
    return dp[n]

# Verify
assert count_partitions(5) == 7
assert count_distinct_partitions(5) == 3  # 5, 4+1, 3+2

for n in range(1, 21):
    print(f"p({n}) = {count_partitions(n)}, q({n}) = {count_distinct_partitions(n)}")
