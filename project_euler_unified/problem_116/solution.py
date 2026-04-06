"""
Problem 116: Red, Green or Blue Tiles

Count ways to tile a row of 50 black squares using tiles of a single color
(red=2, green=3, blue=4) without mixing colors, with at least one colored tile.
"""

def count_tilings(n, L):
    """Count tilings of row length n with black (1) and colored (L) tiles."""
    f = [0] * (n + 1)
    for i in range(min(L, n + 1)):
        f[i] = 1
    for i in range(L, n + 1):
        f[i] = f[i - 1] + f[i - L]
    return f[n]

N = 50
answer = sum(count_tilings(N, L) - 1 for L in [2, 3, 4])
print(answer)
