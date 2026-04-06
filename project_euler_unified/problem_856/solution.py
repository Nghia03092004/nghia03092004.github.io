from functools import lru_cache

@lru_cache(maxsize=None)
def E(n1, n2, n3, n4, c):
    """
    We have already drawn some cards. The last drawn card had rank R with c
    copies remaining in the deck. The other ranks have n1 ranks with 1 card,
    n2 with 2, n3 with 3, n4 with 4 cards remaining.

    Returns the expected number of ADDITIONAL cards to draw until we stop.
    """
    R = n1 + 2*n2 + 3*n3 + 4*n4 + c  # total remaining cards
    if R == 0:
        return 0.0  # deck exhausted, already stopped

    # Draw next card:
    # Prob c/R: matches last card -> stop, draws 1 more card
    # Prob (R-c)/R: doesn't match -> draws 1 card and continues

    match_prob = c / R
    nonmatch_prob = 1.0 - match_prob

    if nonmatch_prob == 0:
        return 1.0  # forced match

    # For non-match: the drawn card comes from some other rank.
    # The old "last rank" (c remaining) goes back to pool.
    # We pick a new card from ranks in pool + returned rank... no wait.
    # The old last rank IS part of the deck but NOT in n1..n4.
    # When we draw a non-matching card, it must come from ranks in n1..n4.
    # After drawing it:
    #   - old last rank (c remaining) returns to n_i counts
    #   - the drawn rank (had j cards, now has j-1) becomes new "last rank" with c'=j-1

    ev_nonmatch = 0.0
    # Total non-matching cards = n1 + 2*n2 + 3*n3 + 4*n4
    total_other = n1 + 2*n2 + 3*n3 + 4*n4
    assert abs(total_other - (R - c)) < 1e-9

    for j in range(1, 5):
        nj = [0, n1, n2, n3, n4][j]
        if nj == 0:
            continue
        prob_j = (j * nj) / total_other  # prob of drawing from a rank with j cards

        # New state:
        # - Old last rank returns to pool: if c > 0, add to n_c
        # - Drawn rank: had j cards, now has j-1, becomes new last rank
        # - Remove drawn rank from pool (decrement n_j)
        new_n1, new_n2, new_n3, new_n4 = n1, n2, n3, n4

        # Remove the drawn rank from its group
        if j == 1: new_n1 -= 1
        elif j == 2: new_n2 -= 1
        elif j == 3: new_n3 -= 1
        else: new_n4 -= 1

        # Return old last rank to pool
        if c == 1: new_n1 += 1
        elif c == 2: new_n2 += 1
        elif c == 3: new_n3 += 1

        new_c = j - 1

        ev_nonmatch += prob_j * E(new_n1, new_n2, new_n3, new_n4, new_c)

    return 1.0 + nonmatch_prob * ev_nonmatch

# Test with 4-card deck (2 ranks, 2 suits)
# After drawing first card: 1 rank with 1 card left (last drawn), 1 rank with 2 cards
# n1=0, n2=1, n3=0, n4=0, c=1
test = 1.0 + E(0, 1, 0, 0, 1)
print(f"4-card deck: {test:.8f} (expected 3.0)")

# Full 52-card deck
# After drawing first card: 12 ranks with 4 cards, c=3
answer = 1.0 + E(0, 0, 0, 12, 3)
print(f"{answer:.8f}")
