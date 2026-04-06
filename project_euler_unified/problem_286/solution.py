"""
Problem 286: Scoring Probabilities

Find q such that the probability of scoring exactly 20 out of 50 shots
equals 0.02, where shot k scores with probability (1 - k/q).
"""

from decimal import Decimal, getcontext

getcontext().prec = 50

def compute_prob(q):
    """Compute P(exactly 20 scores) given q, using DP."""
    dp = [Decimal(0)] * 21
    dp[0] = Decimal(1)
    for i in range(1, 51):
        p_score = Decimal(1) - Decimal(i) / q
        p_miss = Decimal(i) / q
        for j in range(min(i, 20), 0, -1):
            dp[j] = dp[j] * p_miss + dp[j - 1] * p_score
        dp[0] *= p_miss
    return dp[20]

def solve():
    target = Decimal("0.02")
    lo = Decimal("50")
    hi = Decimal("100")

    for _ in range(200):
        mid = (lo + hi) / 2
        val = compute_prob(mid)
        if val > target:
            lo = mid
        else:
            hi = mid

    q = (lo + hi) / 2
    print(f"{q:.10f}")

if __name__ == "__main__":
    solve()

    # Optional visualization
