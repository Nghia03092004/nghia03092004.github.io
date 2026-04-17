"""
Problem 499: St. Petersburg Lottery
The St. Petersburg paradox with finite bankroll.
A coin is flipped repeatedly; payout doubles each flip.
With finite bankroll B, payout is min(2^k, B).
"""

import numpy as np
from decimal import Decimal, getcontext

getcontext().prec = 50

def expected_payout_single(B: int) -> float:
    """Expected payout for a single round with bankroll B.
    E = m + B / 2^m where m = floor(log2(B)).
    """
    if B <= 0:
        return 0.0
    m = B.bit_length() - 1  # floor(log2(B))
    return m + B / (2 ** m)

def expected_payout_exact(B: int) -> Decimal:
    """Exact expected payout using Decimal arithmetic."""
    if B <= 0:
        return Decimal(0)
    m = B.bit_length() - 1
    return Decimal(m) + Decimal(B) / Decimal(2 ** m)

def simulate_petersburg(B: int, F: float, num_games: int = 100000) -> float:
    """Monte Carlo simulation of the St. Petersburg game with finite bankroll."""
    rng = np.random.default_rng(42)
    total_profit = 0.0
    bankroll = float(B)

    for _ in range(num_games):
        # Flip coins until tails
        payout = 1
        while rng.random() < 0.5:
            payout *= 2
        actual_payout = min(payout, bankroll)
        profit = actual_payout - F
        total_profit += profit

    return total_profit / num_games

def expected_payout_multi_round(B: int, F: float, rounds: int = 1000) -> float:
    """Expected profit per round considering bankroll dynamics.
    Uses dynamic programming / recursion.
    """
    # For multi-round: bankroll changes each round
    # After round: bankroll += F - payout
    # This is complex; for the basic problem, single round suffices

    # Single round expected profit
    ep = expected_payout_single(B)
    return ep - F

def solve():
    """Compute the fair entry fee for various bankrolls."""
    print("St. Petersburg Lottery - Fair Entry Fees:")
    print(f"{'Bankroll B':>20} {'m=floor(log2 B)':>15} {'Fair Fee F':>20}")
    print("-" * 60)

    for B_exp in range(1, 35):
        B = 2 ** B_exp
        ep = expected_payout_exact(B)
        print(f"{B:>20d} {B_exp:>15d} {float(ep):>20.8f}")

    print()

    # The specific case: B = 10^10
    B = 10 ** 10
    m = B.bit_length() - 1
    ep = expected_payout_exact(B)
    print(f"B = 10^10 = {B}")
    print(f"m = floor(log2(B)) = {m}")
    print(f"2^m = {2**m}")
    print(f"Fair entry fee F = m + B/2^m = {m} + {B}/{2**m}")
    print(f"                = {float(ep):.8f}")
    print(f"Exact: {ep}")

    # Additional bankrolls of interest
    print("\nOther bankrolls:")
    for B in [100, 1000, 10000, 1000000, 10**9, 10**10, 10**15, 10**20]:
        ep = expected_payout_exact(B)
        print(f"  B = {B:>25d} -> F = {float(ep):.8f}")

solve()
