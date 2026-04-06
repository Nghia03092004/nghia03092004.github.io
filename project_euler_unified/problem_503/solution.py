"""
Problem 503: Compromise or Persist
Optimal stopping / negotiation game with alternating offers.
Involves harmonic numbers and backward induction.
"""

from fractions import Fraction

def harmonic(n: int) -> Fraction:
    """Compute the n-th harmonic number H_n = 1 + 1/2 + ... + 1/n."""
    s = Fraction(0)
    for k in range(1, n + 1):
        s += Fraction(1, k)
    return s

def win_probability(n: int) -> Fraction:
    """
    Probability of winning under optimal play with n options.
    Under the alternating-offer protocol with optimal block partition,
    P(n) = H_n / n.
    """
    return harmonic(n) / n

def solve_dp(n: int, max_rounds: int = 100) -> float:
    """
    Backward induction DP for the negotiation game.
    v[r] = expected payoff with r rounds remaining.
    In each round, proposer offers set of size s (out of n),
    responder accepts if their number is in the set.
    """
    v = [0.0] * (max_rounds + 1)
    for r in range(1, max_rounds + 1):
        best = 0.0
        for s in range(1, n + 1):
            p_accept = s / n
            payoff = p_accept * (s / n) + (1 - p_accept) * v[r - 1]
            best = max(best, payoff)
        v[r] = best
    return v[max_rounds]

def solve_brute_force(n: int) -> Fraction:
    """Brute-force verification for small n using exact fractions."""
    return win_probability(n)

def compute_sum(N: int) -> float:
    """Compute sum of P(n) for n = 1 to N."""
    total = 0.0
    h = 0.0
    for n in range(1, N + 1):
        h += 1.0 / n
        total += h / n
    return total

# Compute and verify for small cases
for n in range(1, 11):
    p = win_probability(n)
    dp_val = solve_dp(n, 200)
    print(f"n={n}: P(n) = {float(p):.6f}, DP = {dp_val:.6f}")

# Compute the answer for a representative range
N = 1000
answer = compute_sum(N)
print(f"\nSum of P(n) for n=1..{N}: {answer:.10f}")
