"""
Problem 611: Hallway of Square Steps
Count ways to traverse distance n using steps of perfect square lengths.
"""

def count_square_step_ways(n):
    """Count ordered ways to sum to n using perfect squares."""
    dp = [0] * (n + 1)
    dp[0] = 1
    for i in range(1, n + 1):
        k = 1
        while k * k <= i:
            dp[i] += dp[i - k * k]
            k += 1
    return dp[n]

# Also count representations as sum of distinct squares
def count_distinct_square_sums(n):
    """Count ways to write n as sum of distinct perfect squares."""
    squares = []
    k = 1
    while k * k <= n:
        squares.append(k * k)
        k += 1
    
    dp = [0] * (n + 1)
    dp[0] = 1
    for sq in squares:
        for j in range(n, sq - 1, -1):
            dp[j] += dp[j - sq]
    return dp[n]

# Verify
assert count_square_step_ways(1) == 1  # 1
assert count_square_step_ways(2) == 1  # 1+1
assert count_square_step_ways(4) == 2  # 4 or 1+1+1+1 or 1+1+1+1... 
# Actually: 4, 1+1+1+1 = 2 (ordered: more ways)
# Ordered: for n=4: {4}, {1,1,1,1}, but also {1,1,1,1} is one way...
# Wait, ordered sums: 4 can be reached as: 4; 1+1+1+1. So dp[4] should be more
c4 = count_square_step_ways(4)
print(f"Ways to reach 4: {c4}")

for n in range(1, 21):
    print(f"  n={n}: {count_square_step_ways(n)} ordered ways")
