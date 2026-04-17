"""
Problem 952: Integer Points on Ellipses

Count integer (x, y) with 3x^2 + 5y^2 <= N, where N = 10^6.

The lattice point count for ellipse Ax^2 + By^2 <= N is approximately
pi*N/sqrt(AB), with error O(sqrt(N)). We compute exactly by slicing:
for each valid x, count y-values satisfying y^2 <= (N - 3x^2) / 5.

Complexity: O(sqrt(N)) time, O(1) space.
"""

import numpy as np
from math import isqrt

def solve(N: int = 10**6) -> int:
    """Count integer points (x, y) with 3x^2 + 5y^2 <= N."""
    count = 0
    max_x = isqrt(N // 3)
    for x in range(-max_x, max_x + 1):
        rem = N - 3 * x * x
        if rem < 0:
            continue
        max_y = isqrt(rem // 5)
        # Verify floor correctness
        while 5 * max_y * max_y > rem:
            max_y -= 1
        count += 2 * max_y + 1
    return count

# --- Compute answer ---
N = 10**6
answer = solve(N)
area_approx = np.pi * N / np.sqrt(15)
print(f"Exact count:     {answer}")
print(f"Area estimate:   {area_approx:.1f}")
print(f"Relative error:  {abs(answer - area_approx) / area_approx:.4%}")
print(answer)
