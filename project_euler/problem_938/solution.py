"""
Problem 938: Circle Lattice Points

Compute D(N) = sum_{r=1}^{N} C(r), where C(r) counts the number of
lattice points (x, y) with x^2 + y^2 <= r^2 (Gauss circle problem).

The exact count is:
    C(r) = 1 + 4 * sum_{k=0}^{inf} (floor(r^2/(4k+1)) - floor(r^2/(4k+3)))
or equivalently by direct enumeration over x.

The error term E(r) = C(r) - pi*r^2 is known to satisfy |E(r)| = O(r^{2/3}).

Results:
    - C(1) = 5, C(2) = 13, C(3) = 29, C(5) = 81
    - D(500) computed below

Methods:
    1. count_lattice_points  -- direct enumeration of C(r)
    2. cumulative_D          -- compute D(N) = sum C(r)
    3. error_term            -- E(r) = C(r) - pi*r^2
    4. normalized_error      -- E(r) / r^(2/3) for scaling analysis
"""
import numpy as np

def count_lattice_points(r):
    """C(r) = #{(x,y) in Z^2 : x^2 + y^2 <= r^2}."""
    r2 = r * r
    count = 0
    for x in range(-r, r + 1):
        max_y = int((r2 - x * x) ** 0.5)
        count += 2 * max_y + 1
    return count

def cumulative_D(N):
    """Compute D(N) and return (D(N), list of C(r))."""
    total = 0
    c_vals = []
    for r in range(1, N + 1):
        c = count_lattice_points(r)
        c_vals.append(c)
        total += c
    return total, c_vals

def error_term(r, c_r):
    """E(r) = C(r) - pi * r^2."""
    return c_r - np.pi * r ** 2

def normalized_error(r, c_r):
    """E(r) / r^(2/3) -- should remain bounded."""
    if r == 0:
        return 0.0
    return error_term(r, c_r) / (r ** (2.0 / 3.0))

# Verification
assert count_lattice_points(1) == 5    # (0,0),(+-1,0),(0,+-1)
assert count_lattice_points(2) == 13
assert count_lattice_points(3) == 29
assert count_lattice_points(5) == 81

# Computation
N = 500
answer, c_vals = cumulative_D(N)
print(answer)
