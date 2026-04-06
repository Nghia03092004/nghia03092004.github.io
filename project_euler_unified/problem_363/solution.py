"""
Problem 363: Bezier Curves

Find optimal cubic Bezier approximation to a quarter circle.
Minimize the maximum radial deviation from the unit circle.

By symmetry, control points are:
  P0 = (0,1), P1 = (k,1), P2 = (1,k), P3 = (1,0)

We optimize k to minimize max |sqrt(x^2+y^2) - 1| over t in [0,1].

Answer: 0.0000372091
"""

import math
from scipy.optimize import minimize_scalar

def bezier(t, k):
    """Evaluate cubic Bezier at parameter t with symmetry parameter k."""
    s = 1.0 - t
    x = 3 * s * s * t * k + 3 * s * t * t * 1.0 + t ** 3
    y = s ** 3 + 3 * s * s * t * 1.0 + 3 * s * t * t * k
    return x, y

def radial_error(t, k):
    """Radial deviation from unit circle at parameter t."""
    x, y = bezier(t, k)
    return math.sqrt(x * x + y * y) - 1.0

def max_abs_error(k):
    """Maximum absolute radial error over t in [0, 1]."""
    N = 100000
    max_err = 0.0
    for i in range(N + 1):
        t = i / N
        err = abs(radial_error(t, k))
        if err > max_err:
            max_err = err
    return max_err

def solve():
    # Golden section search for optimal k
    result = minimize_scalar(max_abs_error, bounds=(0.5, 0.6), method='bounded',
                             options={'xatol': 1e-15})
    optimal_k = result.x
    min_max_dev = max_abs_error(optimal_k)
    return min_max_dev

if __name__ == "__main__":
    answer = solve()
    print(f"{answer:.10f}")
    # Expected: 0.0000372091
