"""
Project Euler Problem 161: Triominoes

Count the number of ways to tile a 9x12 grid with triominoes
(both L-shaped and I-shaped) using broken-profile dynamic programming.
"""

from collections import defaultdict


def solve():
    ROWS, COLS = 9, 12

    # All 6 triomino placements anchored at (0,0) in row-major order:
    #   L-A: (0,0),(0,1),(1,0)     L-B: (0,0),(0,1),(1,1)
    #   L-C: (0,0),(1,0),(1,1)     L-D: (0,0),(1,-1),(1,0)
    #   I-H: (0,0),(0,1),(0,2)     I-V: (0,0),(1,0),(2,0)
    triominoes = [
        [(0, 0), (0, 1), (1, 0)],
        [(0, 0), (0, 1), (1, 1)],
        [(0, 0), (1, 0), (1, 1)],
        [(0, 0), (1, -1), (1, 0)],
        [(0, 0), (0, 1), (0, 2)],
        [(0, 0), (1, 0), (2, 0)],
    ]

    dp = defaultdict(int)
    dp[0] = 1

    for r in range(ROWS):
        for c in range(COLS):
            ndp = defaultdict(int)
            for mask, ways in dp.items():
                if mask & 1:
                    ndp[mask >> 1] += ways
                else:
                    for tri in triominoes:
                        ok = True
                        nmask = mask
                        for dr, dc in tri:
                            nr, nc = r + dr, c + dc
                            if nr < 0 or nr >= ROWS or nc < 0 or nc >= COLS:
                                ok = False
                                break
                            bit = dr * COLS + dc
                            if bit < 0 or nmask & (1 << bit):
                                ok = False
                                break
                            nmask |= 1 << bit
                        if ok:
                            ndp[nmask >> 1] += ways
            dp = ndp

    print(dp.get(0, 0))


solve()
