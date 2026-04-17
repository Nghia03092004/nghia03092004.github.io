"""
Problem 220: Heighway Dragon

L-system: Fa -> Fa R Fb, Fb -> Fa L Fb
Start at origin (0,0) facing north (up).
Find position after 10^12 drawing steps at expansion level 50.

Approach: Precompute net displacement and rotation for each level/symbol/direction,
then recursively decompose the query.

Answer: 139776,963904
"""

import sys
sys.setrecursionlimit(100)

MAXN = 51

# Directions: 0=N(+y), 1=E(+x), 2=S(-y), 3=W(-x)
DX = [0, 1, 0, -1]
DY = [1, 0, -1, 0]

def solve():
    # Precompute displacement and rotation tables
    # disp_x[n][s][d], disp_y[n][s][d], rot[n][s]
    disp_x = [[[0]*4 for _ in range(2)] for _ in range(MAXN)]
    disp_y = [[[0]*4 for _ in range(2)] for _ in range(MAXN)]
    rot = [[0]*2 for _ in range(MAXN)]

    # Base case: level 0
    for s in range(2):
        rot[0][s] = 0
        for d in range(4):
            disp_x[0][s][d] = DX[d]
            disp_y[0][s][d] = DY[d]

    # Build up
    for n in range(1, MAXN):
        # Fa^(n) = Fa^(n-1) R Fb^(n-1)
        rot[n][0] = (rot[n-1][0] + 1 + rot[n-1][1]) % 4
        # Fb^(n) = Fa^(n-1) L Fb^(n-1)
        rot[n][1] = (rot[n-1][0] - 1 + rot[n-1][1]) % 4

        for d in range(4):
            # Fa^(n)
            x1 = disp_x[n-1][0][d]
            y1 = disp_y[n-1][0][d]
            d_after = (d + rot[n-1][0]) % 4
            d_second = (d_after + 1) % 4  # R = +1
            x2 = disp_x[n-1][1][d_second]
            y2 = disp_y[n-1][1][d_second]
            disp_x[n][0][d] = x1 + x2
            disp_y[n][0][d] = y1 + y2

            # Fb^(n)
            x1 = disp_x[n-1][0][d]
            y1 = disp_y[n-1][0][d]
            d_after = (d + rot[n-1][0]) % 4
            d_second = (d_after - 1) % 4  # L = -1
            x2 = disp_x[n-1][1][d_second]
            y2 = disp_y[n-1][1][d_second]
            disp_x[n][1][d] = x1 + x2
            disp_y[n][1][d] = y1 + y2

    def query(n, s, d, k):
        """Position after k steps of symbol s at level n, starting direction d."""
        if k == 0:
            return (0, 0)
        if n == 0:
            return (DX[d], DY[d])

        half = 1 << (n - 1)

        if k <= half:
            return query(n - 1, 0, d, k)
        else:
            x1 = disp_x[n-1][0][d]
            y1 = disp_y[n-1][0][d]
            d_after = (d + rot[n-1][0]) % 4
            turn = 1 if s == 0 else -1  # R for a, L for b
            d_second = (d_after + turn) % 4
            x2, y2 = query(n - 1, 1, d_second, k - half)
            return (x1 + x2, y1 + y2)

    steps = 10**12
    level = 50
    x, y = query(level, 0, 0, steps)
    print(f"{x},{y}")

if __name__ == "__main__":
    solve()
