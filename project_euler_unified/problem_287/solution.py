"""
Problem 287: Quadtree Encoding

Find the shortest quadtree encoding length for a 2^N x 2^N image (N=24)
where pixel (x,y) is black iff (x - 2^(N-1))^2 + (y - 2^(N-1))^2 <= 2^(2(N-1)).
"""

import sys
sys.setrecursionlimit(1000000)

N = 24
C = 1 << (N - 1)       # center coordinate
R2 = 1 << (2 * (N - 1))  # radius squared

def inside(x, y):
    dx = x - C
    dy = y - C
    return dx * dx + dy * dy <= R2

def solve(x0, y0, s):
    if s == 1:
        return 2

    x1 = x0 + s - 1
    y1 = y0 + s - 1

    # Check if all corners are inside (convexity => all pixels inside)
    if (inside(x0, y0) and inside(x1, y0) and
        inside(x0, y1) and inside(x1, y1)):
        return 2

    # Check if entirely outside: closest point to center
    cx = max(x0, min(C, x1))
    cy = max(y0, min(C, y1))
    if not inside(cx, cy):
        return 2

    # Mixed: subdivide
    h = s // 2
    return (1 + solve(x0, y0, h) + solve(x0 + h, y0, h) +
            solve(x0, y0 + h, h) + solve(x0 + h, y0 + h, h))

def main():
    S = 1 << N
    result = solve(0, 0, S)
    print(result)

if __name__ == "__main__":
    main()
