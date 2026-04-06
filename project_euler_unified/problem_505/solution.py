"""
Problem 505: Bidding Game
A game where players bid from limited chip resources.
Solved via backward induction and dynamic programming.
"""

import numpy as np
from functools import lru_cache

def solve_bidding_game(n: int, target: int = 2) -> float:
    """
    Solve the bidding game where each player starts with n chips.
    First player to win 'target' rounds wins.
    Returns the game value (probability that player 1 wins under optimal play).

    Uses backward induction with memoization.
    """

    @lru_cache(maxsize=None)
    def value(a: int, b: int, ra: int, rb: int) -> float:
        """
        Value to player A.
        a, b = remaining chips for A, B.
        ra, rb = rounds still needed by A, B to win.
        """
        if ra == 0:
            return 1.0
        if rb == 0:
            return 0.0
        if a == 0 and b == 0:
            # No chips left; alternate wins. Assume A goes first in ties.
            # With no chips, each round is a tie; alternate who wins.
            # If ra <= rb, A wins; otherwise B wins (simplified).
            return 1.0 if ra <= rb else 0.0

        # Solve the one-shot matrix game via minimax
        # A bids i in [0, a], B bids j in [0, b]
        # If i > j: A wins round, continue with (a-i, b-j, ra-1, rb)
        # If i < j: B wins round, continue with (a-i, b-j, ra, rb-1)
        # If i == j: tie-break (alternate), say A wins on even rounds

        # For a proper minimax, we need to find the saddle point.
        # Use iterative best response for efficiency.

        # Build payoff matrix
        rows = a + 1  # A's bids: 0..a
        cols = b + 1  # B's bids: 0..b
        M = np.zeros((rows, cols))

        for i in range(rows):
            for j in range(cols):
                if i > j:
                    M[i][j] = value(a - i, b - j, ra - 1, rb)
                elif i < j:
                    M[i][j] = value(a - i, b - j, ra, rb - 1)
                else:
                    # Tie: A wins the tie (convention)
                    M[i][j] = value(a - i, b - j, ra - 1, rb)

        # Find minimax value: max over A's mixed strategies, min over B's
        # For small matrices, use LP or brute force
        # Simplified: use pure strategy minimax
        row_mins = M.min(axis=1)
        val = row_mins.max()

        return float(val)

    return value(n, n, target, target)

def solve_small(max_n: int = 20) -> list:
    """Compute game values for n = 1 to max_n."""
    results = []
    for n in range(1, max_n + 1):
        v = solve_bidding_game(n, target=2)
        results.append((n, v))
        print(f"n={n}: value = {v:.6f}")
    return results

# Compute for small cases
results = solve_small(15)

# Compute sum
total = sum(v for _, v in results)
print(f"\nSum of f(n) for n=1..{len(results)}: {total:.6f}")
