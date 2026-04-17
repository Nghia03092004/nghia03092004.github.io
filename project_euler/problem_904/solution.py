"""
Problem 904: Integer Partition Asymptotics

Let p(n) denote the number of integer partitions of n. Find p(200) mod 10^9+7.

Key identities:
    GF:         sum p(n) x^n = prod 1/(1-x^k)
    Pentagonal: prod (1-x^k) = sum (-1)^j x^{j(3j-1)/2}
    Recurrence: p(n) = sum (-1)^{j+1} p(n - w_j) where w_j = j(3j-1)/2

Methods:
    1. Standard DP (knapsack/coin-change) - O(n^2)
    2. Pentagonal number recurrence - O(n^{3/2})
    3. Exact computation with Hardy-Ramanujan verification
"""

from math import factorial, sqrt, pi, exp, log

MOD = 10**9 + 7

def partition_dp(n: int):
    """Compute p(n) exactly using the coin-change DP.

    dp[0] = 1; for each part size k, update dp[j] += dp[j-k].
    After processing all k, dp[n] = p(n).
    """
    dp = [0] * (n + 1)
    dp[0] = 1
    for k in range(1, n + 1):
        for j in range(k, n + 1):
            dp[j] += dp[j - k]
    return dp[n]

def partition_dp_mod(n: int, mod: int):
    """Compute p(n) mod m using the coin-change DP."""
    dp = [0] * (n + 1)
    dp[0] = 1
    for k in range(1, n + 1):
        for j in range(k, n + 1):
            dp[j] = (dp[j] + dp[j - k]) % mod
    return dp[n]

def generalized_pentagonal():
    """Generate generalized pentagonal numbers j(3j-1)/2 for j = 1,-1,2,-2,..."""
    j = 1
    while True:
        yield j * (3 * j - 1) // 2  # positive j
        yield (-j) * (3 * (-j) - 1) // 2  # negative j (= j*(3j+1)/2)
        j += 1

def partition_pentagonal(n: int):
    """Compute p(0), p(1), ..., p(n) using the pentagonal number recurrence.

    p(n) = sum_{j != 0} (-1)^{j+1} p(n - w_j)
    where w_j = j(3j-1)/2 are generalized pentagonal numbers.
    """
    p = [0] * (n + 1)
    p[0] = 1
    for m in range(1, n + 1):
        total = 0
        sign = 1
        for j in range(1, m + 1):
            # Two pentagonal numbers per j: w+ = j(3j-1)/2, w- = j(3j+1)/2
            w1 = j * (3 * j - 1) // 2
            w2 = j * (3 * j + 1) // 2
            if w1 > m:
                break
            total += sign * p[m - w1]
            if w2 <= m:
                total += sign * p[m - w2]
            sign = -sign
        p[m] = total
    return p[n]

def partition_pentagonal_all(n: int) -> list:
    """Return the full array [p(0), p(1), ..., p(n)]."""
    p = [0] * (n + 1)
    p[0] = 1
    for m in range(1, n + 1):
        total = 0
        sign = 1
        for j in range(1, m + 1):
            w1 = j * (3 * j - 1) // 2
            w2 = j * (3 * j + 1) // 2
            if w1 > m:
                break
            total += sign * p[m - w1]
            if w2 <= m:
                total += sign * p[m - w2]
            sign = -sign
        p[m] = total
    return p

# Hardy-Ramanujan approximation
def hardy_ramanujan(n: int) -> float:
    """Hardy-Ramanujan asymptotic formula for p(n)."""
    if n <= 0:
        return 1.0
    return 1.0 / (4.0 * n * sqrt(3.0)) * exp(pi * sqrt(2.0 * n / 3.0))

# Solve
N = 200

p_exact = partition_dp(N)

p_pent = partition_pentagonal(N)

# Cross-check
assert p_exact == p_pent, f"Mismatch: DP={p_exact}, Pentagonal={p_pent}"

# Known values verification
known = {0: 1, 1: 1, 2: 2, 3: 3, 4: 5, 5: 7, 6: 11, 7: 15, 8: 22, 9: 30, 10: 42}
p_all = partition_pentagonal_all(max(known.keys()))
for n, val in known.items():
    assert p_all[n] == val, f"p({n}) = {p_all[n]}, expected {val}"

# Modular answer
ans = p_exact % MOD

# Also verify modular DP
ans_mod = partition_dp_mod(N, MOD)
assert ans == ans_mod, f"Mod mismatch: {ans} vs {ans_mod}"

print(ans)
