"""
Problem 908: Coprime Chains

Find the longest coprime chain in {2, 3, ..., 100}, where consecutive
elements must be coprime.

Key insight: gcd(n, n+1) = 1, so the full sequence 2,3,...,100 is valid.

Methods:
    1. Direct observation (consecutive integers are coprime)
    2. DP on the coprime graph (verification)
    3. Brute-force longest path (small N)
"""

from math import gcd

def solve_direct(N: int):
    """Consecutive integers are coprime, so max chain = N - 1."""
    return N - 1

def solve_dp(N: int):
    """Longest increasing coprime chain via DP.

    dp[i] = length of longest coprime chain ending at i.
    dp[i] = 1 + max{dp[j] : j < i, gcd(j, i) = 1}
    """
    dp = [0] * (N + 1)
    for i in range(2, N + 1):
        dp[i] = 1
        for j in range(2, i):
            if gcd(j, i) == 1:
                dp[i] = max(dp[i], dp[j] + 1)
    return max(dp[2:])

def verify_consecutive(N: int) -> bool:
    """Check that gcd(k, k+1) = 1 for all k in [2, N-1]."""
    return all(gcd(k, k + 1) == 1 for k in range(2, N))

# Solve
N = 100
ans = solve_direct(N)

# Verify with DP for smaller N
for test_n in [5, 10, 20, 50]:
    dp_ans = solve_dp(test_n)
    direct_ans = solve_direct(test_n)
    assert dp_ans == direct_ans, f"N={test_n}: DP={dp_ans}, direct={direct_ans}"

# Verify consecutive coprimality
assert verify_consecutive(N)

print(ans)
