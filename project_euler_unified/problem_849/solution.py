"""
Problem 849: The Locked Box

Coupon collector problem: E[T] = n * H_n.
Set cover problem: bitmask DP for exact minimum.
"""

from math import log
from fractions import Fraction

# --- Method 1: Exact expected value via harmonic numbers ---
def coupon_collector_exact(n: int) -> Fraction:
    """Exact expected number of trials: n * H_n."""
    return n * sum(Fraction(1, k) for k in range(1, n + 1))

def coupon_collector_float(n: int) -> float:
    """Float approximation."""
    return n * sum(1.0 / k for k in range(1, n + 1))

# --- Method 2: Monte Carlo simulation ---
def coupon_collector_mc(n: int, trials: int = 100000) -> float:
    """Monte Carlo estimate of E[T]."""
    import random
    total = 0
    for _ in range(trials):
        collected = set()
        steps = 0
        while len(collected) < n:
            collected.add(random.randint(0, n - 1))
            steps += 1
        total += steps
    return total / trials

# --- Method 3: Set cover via bitmask DP ---
def min_set_cover(n: int, sets: list, costs: list = None):
    """Minimum cost to cover all n elements using given sets.
    sets[i] is a frozenset of elements covered by key i.
    costs[i] is the cost of key i (default 1).
    """
    if costs is None:
        costs = [1] * len(sets)
    full = (1 << n) - 1
    INF = float('inf')
    dp = [INF] * (full + 1)
    dp[0] = 0

    # Convert sets to bitmasks
    masks = []
    for s in sets:
        mask = 0
        for elem in s:
            mask |= (1 << elem)
        masks.append(mask)

    for state in range(full + 1):
        if dp[state] == INF:
            continue
        for i, mask in enumerate(masks):
            new_state = state | mask
            if dp[new_state] > dp[state] + costs[i]:
                dp[new_state] = dp[state] + costs[i]
    return dp[full]

# --- Method 4: Variance computation ---
def coupon_collector_variance(n: int) -> float:
    return n**2 * sum(1.0/k**2 for k in range(1, n + 1)) - n * sum(1.0/k for k in range(1, n + 1))

# --- Verification ---
assert coupon_collector_exact(1) == 1
assert coupon_collector_exact(2) == 3
assert abs(coupon_collector_float(10) - 29.2897) < 0.01

# Set cover verification
sets = [{0, 1}, {1, 2}, {0, 2}]
assert min_set_cover(3, sets) == 2  # need at least 2 sets

# MC should be close to exact
mc = coupon_collector_mc(10, 50000)
assert abs(mc - 29.29) < 1.0, f"MC estimate {mc} too far from 29.29"

print("Verification passed!")

MOD = 10**9 + 7
# Compute nH_n mod p using modular inverse
def harmonic_mod(n, mod):
    total = 0
    for k in range(1, n + 1):
        total = (total + pow(k, mod - 2, mod)) % mod
    return total

answer = 1000 * harmonic_mod(1000, MOD) % MOD
print(f"Answer: {answer}")
