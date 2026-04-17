"""
Problem 210: Obtuse Angled Triangles

S(r) = lattice points with |x|+|y| <= r. O = (0,0), C = (r/4, r/4).
Count B in S(r) such that triangle OBC is obtuse.

Three disjoint obtuse regions:
- Region O (obtuse at O): x+y < 0, count = r^2
- Region C (obtuse at C): x+y > r/2, count = r^2/2
- Region B (obtuse at B): (x-r/8)^2 + (y-r/8)^2 < r^2/32, x != y
  = circle_count - (r/4 - 1)

N(r) = 3r^2/2 + circle_count - r/4 + 1

Note: For r = 10^9, the circle loop has ~1.77*10^8 iterations.
This runs in ~2-3 minutes in Python (or a few seconds in C++).
"""

import math

def solve(r):
    assert r % 8 == 0

    region_OC = r * r + r * r // 2  # r^2 + r^2/2

    s = r // 8  # center coordinate
    R2 = 2 * s * s  # radius squared = r^2/32

    # Count lattice points (u,v) with u^2 + v^2 < R2
    max_u = math.isqrt(R2 - 1)

    circle_count = 0
    # u = 0
    rem = R2
    q = math.isqrt(rem)
    mv = q - 1 if q * q == rem else q
    circle_count += 2 * mv + 1

    # u = 1 to max_u, counted twice (for -u)
    for u in range(1, max_u + 1):
        rem = R2 - u * u
        if rem <= 0:
            break
        q = math.isqrt(rem)
        mv = q - 1 if q * q == rem else q
        circle_count += 2 * (2 * mv + 1)

    collinear = r // 4 - 1
    region_B = circle_count - collinear

    answer = region_OC + region_B
    return answer

# For r = 10^9, this takes a few minutes in Python.
# Uncomment the line below to run (or use the C++ version for speed).
r = 10**9
print(solve(r))
