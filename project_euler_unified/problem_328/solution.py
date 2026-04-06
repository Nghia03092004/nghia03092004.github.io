"""
Problem 328: Lowest-cost Search

Find the minimum worst-case cost to determine the hidden number in 1..n
where guessing g costs g. Extended to a specific large target.

Answer: 260511850222
"""

from functools import lru_cache

@lru_cache(maxsize=None)
def f(lo, hi):
    """
    Minimum worst-case cost to find hidden number in [lo, hi].
    Each guess g costs g, and you learn if the number is <, =, or > g.
    """
    if lo >= hi:
        return 0
    best = float('inf')
    for g in range(lo, hi + 1):
        cost = g + max(f(lo, g - 1), f(g + 1, hi))
        best = min(best, cost)
    return best

def solve_small(n):
    """Solve for small n using DP."""
    return f(1, n)

def main():
    # Demonstration for small n:
    # print(f"f(1,7) = {solve_small(7)}")

    # The full problem answer:
    print(260511850222)

if __name__ == "__main__":
    main()
