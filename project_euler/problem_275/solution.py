"""
Problem 275: Balanced Sculptures

Order n = n blocks + 1 plinth = n+1 tiles.
Plinth at (0,0). Blocks at y >= 1. Connected polyomino.
x_sum of blocks = 0. Reflections about y-axis identified.

Uses Redelmeier's algorithm with excluded array for permanently skipped cells.

Note: This Python version is significantly slower than the C++ version.
For ORDER=18, use the C++ solution.
"""

import sys
sys.setrecursionlimit(1000000)

ORDER = 18
MAXC = 20
W = 2 * MAXC + 1
H = MAXC + 1

DX = [0, 0, -1, 1]
DY = [-1, 1, 0, 0]

def encode(x, y):
    return y * W + (x + MAXC)

def dec_x(c):
    return c % W - MAXC

def dec_y(c):
    return c // W

MAXCELLS = H * W
placed = [False] * MAXCELLS
excluded = [False] * MAXCELLS
in_untried = [False] * MAXCELLS

result_total = 0
result_sym = 0

cells_arr = [0] * 25
n_placed = 0
x_sum = 0

def dfs(untried):
    global n_placed, x_sum, result_total, result_sym

    if n_placed == ORDER:
        if x_sum == 0:
            result_total += 1
            sym = True
            for i in range(ORDER):
                x = dec_x(cells_arr[i])
                y = dec_y(cells_arr[i])
                mirror = encode(-x, y)
                if not placed[mirror]:
                    sym = False
                    break
            if sym:
                result_sym += 1
        return

    remaining = ORDER - n_placed
    if abs(x_sum) > remaining * MAXC:
        return
    if not untried:
        return

    c = untried[0]
    cx = dec_x(c)
    cy = dec_y(c)

    # Branch 1: Add c
    placed[c] = True
    in_untried[c] = False
    cells_arr[n_placed] = c
    n_placed += 1
    x_sum += cx

    added = []
    for d in range(4):
        nx = cx + DX[d]
        ny = cy + DY[d]
        if ny < 1 or ny > MAXC or nx < -MAXC or nx > MAXC:
            continue
        nc = encode(nx, ny)
        if not placed[nc] and not excluded[nc] and not in_untried[nc]:
            in_untried[nc] = True
            added.append(nc)

    added.sort()
    new_untried = []
    ai = 0
    for i in range(1, len(untried)):
        while ai < len(added) and added[ai] < untried[i]:
            new_untried.append(added[ai])
            ai += 1
        new_untried.append(untried[i])
    while ai < len(added):
        new_untried.append(added[ai])
        ai += 1

    dfs(new_untried)

    n_placed -= 1
    x_sum -= cx
    placed[c] = False
    for nc in added:
        in_untried[nc] = False

    # Branch 2: Skip c
    excluded[c] = True
    in_untried[c] = False
    dfs(untried[1:])
    excluded[c] = False
    in_untried[c] = True

def solve():
    global n_placed, x_sum

    # Place plinth at (0, 0)
    placed[encode(0, 0)] = True
    n_placed = 0
    x_sum = 0

    # Initial untried: y >= 1 neighbors of (0, 0) = just (0, 1)
    init_untried = []
    for d in range(4):
        nx, ny = DX[d], DY[d]
        if ny < 1 or ny > MAXC or nx < -MAXC or nx > MAXC:
            continue
        nc = encode(nx, ny)
        init_untried.append(nc)
        in_untried[nc] = True
    init_untried.sort()

    dfs(init_untried)

    answer = (result_total + result_sym) // 2
    print(answer)

if __name__ == "__main__":
    solve()
