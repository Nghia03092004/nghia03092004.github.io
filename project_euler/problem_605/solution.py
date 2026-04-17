"""
Problem 605: Pairwise Coin Tossing Game
Round-robin tournament with coin tosses. Probability analysis.
"""
from itertools import product
from fractions import Fraction

def tournament_probs(n):
    """Compute probability that player 0 wins all games in round-robin of n players.
    Each game is fair coin: P(win) = 1/2.
    P(player 0 beats all) = (1/2)^(n-1).
    """
    return Fraction(1, 2**(n-1))

def prob_unique_winner(n):
    """Probability that exactly one player beats all others."""
    # By symmetry, P(some player beats all) = n * (1/2)^(n-1)
    # But for n > 2, overlap is possible (though rare)
    # For n=2: P = 1 (always one winner)
    # Approximation: n / 2^(n-1)
    return Fraction(n, 2**(n-1))

# Verify
assert tournament_probs(2) == Fraction(1, 2)
assert tournament_probs(3) == Fraction(1, 4)

for n in range(2, 11):
    p = tournament_probs(n)
    pu = prob_unique_winner(n)
    print(f"n={n}: P(player 0 wins all)={p}, P(any unique winner)={pu} ({float(pu):.4f})")
