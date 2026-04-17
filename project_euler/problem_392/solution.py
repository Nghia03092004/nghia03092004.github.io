"""
Problem 392: Enmeshed Unit Circle

A rectilinear grid of (N+2) vertical and (N+2) horizontal lines is placed
with outer boundaries at x=+-1, y=+-1. Grid cells overlapping the unit
circle are colored red. Position the N inner gridlines to minimize the
total red cell area.

Approach: By fourfold symmetry, optimize in the first quadrant. The red
cells form an outer staircase under y = sqrt(1-x^2). The optimality
condition dA/dx_k = 0 yields a recurrence:

    x_{k+1} = x_k - (sqrt(1-x_k^2) - sqrt(1-x_{k-1}^2)) * sqrt(1-x_k^2) / x_k

Binary-search on x_1 until the sequence reaches x_{N/2+1} = 1.
"""

import math
import numpy as np

def compute_optimal_positions(n: int):
    """Return the optimal x-partition 0 = x_0 < x_1 < ... < x_{n/2+1} = 1."""
    half = n // 2
    n_strips = half + 1  # number of strips in [0,1]

    lo, up = 1e-15, 1 - 1e-15

    best_xs = None

    for _ in range(500):
        mid = (lo + up) / 2
        xs = [0.0, mid]
        area = mid  # (x1 - 0) * sqrt(1 - 0) = x1

        ok = True
        for i in range(2, n_strips + 1):
            s1 = math.sqrt(1 - xs[-1] ** 2)
            s2 = math.sqrt(1 - xs[-2] ** 2)
            x_next = xs[-1] - (s1 - s2) * s1 / xs[-1]
            xs.append(x_next)
            if x_next > 1:
                ok = False
                break

        if not ok:
            up = mid
        else:
            lo = mid
            best_xs = xs

    return best_xs

def solve(n: int) -> float:
    """Compute A(n), the minimal red cell area for an n-line enmeshed grid."""
    half = n // 2
    n_strips = half + 1

    lo, up = 1e-15, 1 - 1e-15

    for _ in range(500):
        mid = (lo + up) / 2
        x_prev = 0.0
        x_curr = mid
        area = x_curr  # first strip: (x1 - 0) * sqrt(1 - 0^2)

        ok = True
        for i in range(2, n_strips + 1):
            s1 = math.sqrt(1 - x_curr ** 2)
            s2 = math.sqrt(1 - x_prev ** 2)
            x_next = x_curr - (s1 - s2) * s1 / x_curr
            area += (x_next - x_curr) * s1
            x_prev = x_curr
            x_curr = x_next
            if x_curr > 1:
                ok = False
                break

        if not ok:
            up = mid
        else:
            lo = mid

    return 4 * area

def solve_brute_force(n: int) -> float:
    """Verify via scipy optimization for small n."""
    from scipy.optimize import minimize

    half = n // 2
    m = half + 1  # number of strips

    def neg_area(params):
        # params = x_1, ..., x_m (the m inner points, x_0=0 and x_{m+1}=1 implicit)
        xs = np.concatenate(([0.0], np.sort(params), [1.0]))
        area = 0.0
        for i in range(len(xs) - 1):
            area += (xs[i + 1] - xs[i]) * math.sqrt(1 - xs[i] ** 2)
        return area  # minimize this

    # Initial guess: uniform spacing
    x0 = np.linspace(0, 1, m + 2)[1:-1]
    bounds = [(1e-10, 1 - 1e-10)] * m
    result = minimize(neg_area, x0, bounds=bounds, method="L-BFGS-B")
    return 4 * result.fun

# Compute and verify
answer = solve(400)
assert abs(answer - 3.1486734435) < 1e-8, f"Expected 3.1486734435, got {answer:.10f}"

# Cross-check with small cases
a10 = solve(10)
assert abs(a10 - 3.3469640797) < 1e-8, f"A(10) expected 3.3469640797, got {a10:.10f}"

print(f"{answer:.10f}")
