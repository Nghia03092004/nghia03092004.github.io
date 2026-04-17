"""
Problem 208: Robot Walks

A robot takes 70 steps, each an arc of 1/5 of a circle (72 degrees).
At each step it turns left or right. Count distinct closed paths.

Key insight: for the path to close, each of the 5 heading directions must be
visited exactly 14 times (70/5 = 14). We use DP with state
(current_heading, remaining_visits_per_heading).
"""

from functools import lru_cache

@lru_cache(maxsize=None)
def solve(d, c0, c1, c2, c3, c4):
    """Count closed paths from state (heading d, remaining visits c0..c4)."""
    remaining = c0 + c1 + c2 + c3 + c4
    if remaining == 0:
        return 1 if d == 0 else 0

    c = [c0, c1, c2, c3, c4]
    if c[d] == 0:
        return 0  # can't move from this heading if no visits remain

    c[d] -= 1
    result = 0
    # Left turn: heading -> (d+1) % 5
    result += solve((d + 1) % 5, c[0], c[1], c[2], c[3], c[4])
    # Right turn: heading -> (d-1) % 5
    result += solve((d - 1) % 5, c[0], c[1], c[2], c[3], c[4])
    c[d] += 1  # restore (though with lru_cache args are immutable)

    return result

answer = solve(0, 14, 14, 14, 14, 14)
print(answer)
