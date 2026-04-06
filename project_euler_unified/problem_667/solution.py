"""
Problem 667: Moving Pentagon

Find the largest pentagonal table that fits through a unit-wide L-shaped corridor.
Optimization over pentagon parameters with corridor-fitting constraints.
"""

import numpy as np
from scipy.optimize import minimize

def pentagon_area(vertices):
    """Compute area of polygon given vertices using shoelace formula."""
    n = len(vertices)
    area = 0.0
    for i in range(n):
        j = (i + 1) % n
        area += vertices[i][0] * vertices[j][1]
        area -= vertices[j][0] * vertices[i][1]
    return abs(area) / 2.0

def width_in_direction(vertices, theta):
    """Width of polygon perpendicular to direction theta."""
    cos_t, sin_t = np.cos(theta), np.sin(theta)
    projections = [v[0] * cos_t + v[1] * sin_t for v in vertices]
    return max(projections) - min(projections)

def can_fit_corridor(vertices, n_angles=100):
    """Check if pentagon can traverse unit-width L-corridor."""
    for i in range(n_angles + 1):
        theta = i * np.pi / (2 * n_angles)
        w1 = width_in_direction(vertices, theta)
        w2 = width_in_direction(vertices, theta + np.pi / 2)
        if w1 > 1.0 or w2 > 1.0:
            return False
    return True

def max_violation(vertices, n_angles=100):
    """Maximum constraint violation for corridor fitting."""
    max_v = 0.0
    for i in range(n_angles + 1):
        theta = i * np.pi / (2 * n_angles)
        w1 = width_in_direction(vertices, theta)
        w2 = width_in_direction(vertices, theta + np.pi / 2)
        max_v = max(max_v, w1 - 1.0, w2 - 1.0)
    return max_v

# Start with a regular pentagon scaled to fit
def regular_pentagon(scale=0.3):
    """Regular pentagon vertices."""
    angles = [2 * np.pi * k / 5 for k in range(5)]
    return [(scale * np.cos(a), scale * np.sin(a)) for a in angles]

# Simple demonstration
verts = regular_pentagon(0.3)
area = pentagon_area(verts)
fits = can_fit_corridor(verts)
print(f"Regular pentagon (scale=0.3): area = {area:.6f}, fits = {fits}")

# Try larger
verts2 = regular_pentagon(0.45)
area2 = pentagon_area(verts2)
fits2 = can_fit_corridor(verts2)
print(f"Regular pentagon (scale=0.45): area = {area2:.6f}, fits = {fits2}")

# Hammersley-like pentagon approximation
# A pentagon that approximates the moving sofa shape
verts3 = [(-0.5, 0), (-0.5, 0.5), (0, 0.6), (0.5, 0.5), (0.5, 0)]
area3 = pentagon_area(verts3)
fits3 = can_fit_corridor(verts3)
print(f"Sofa-like pentagon: area = {area3:.6f}, fits = {fits3}")

print("\nFull optimization requires SQP solver with fine angle discretization.")
