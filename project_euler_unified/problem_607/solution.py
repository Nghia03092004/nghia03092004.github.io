"""
Project Euler Problem 607: Marsh Crossing

Minimize travel time through 7 regions with different speeds.
Uses Snell's law analogy and scipy optimization.

The marsh runs SW-NE at 45 degrees. We parameterize by the
height (y-coordinate) where the path crosses each marsh boundary.
"""

import numpy as np
from scipy.optimize import minimize

S2 = np.sqrt(2.0)

# Perpendicular distances of boundaries from marsh center
d_boundaries = np.array([-25.0, -15.0, -5.0, 5.0, 15.0, 25.0])
speeds = np.array([10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 10.0])

# A = (0, 0), B = (100, 0)
# Marsh center at (50, 0)
# Perpendicular distance of point (x,y) from center = (x - 50 - y) / sqrt(2)
# Boundary i: x - 50 - y = d_i * sqrt(2), so x = 50 + y + d_i * sqrt(2)

def total_time(h):
    """Compute total travel time given y-coordinates at 6 boundaries."""
    px = np.zeros(8)
    py = np.zeros(8)
    px[0], py[0] = 0.0, 0.0   # A
    px[7], py[7] = 100.0, 0.0  # B
    for i in range(6):
        px[i+1] = 50.0 + h[i] + d_boundaries[i] * S2
        py[i+1] = h[i]

    total = 0.0
    for i in range(7):
        dx = px[i+1] - px[i]
        dy = py[i+1] - py[i]
        dist = np.sqrt(dx**2 + dy**2)
        total += dist / speeds[i]
    return total

# Initial guess: straight path (all h = 0)
h_init = np.zeros(6)

# Use multiple optimizers for precision
result = minimize(total_time, h_init, method='Nelder-Mead',
                  options={'xatol': 1e-15, 'fatol': 1e-15, 'maxiter': 10000000, 'adaptive': True})

result = minimize(total_time, result.x, method='Powell',
                  options={'xtol': 1e-15, 'ftol': 1e-15, 'maxiter': 10000000})

result = minimize(total_time, result.x, method='Nelder-Mead',
                  options={'xatol': 1e-15, 'fatol': 1e-15, 'maxiter': 10000000, 'adaptive': True})

print(f"{result.fun:.10f}")
