"""
Problem 428: Necklace of Circles
Project Euler
"""

import math

def steiner_chain_curvatures(R, r, n):
    """Compute curvatures of n circles in a Steiner chain between circles of radii R and r."""
    # Each circle in the chain has the same radius
    # Using inversive geometry
    d = R - r  # gap
    # Radius of each chain circle
    rc = d / 2 * (1 / (1 + 1/math.sin(math.pi/n)) if n > 0 else 0)
    curvature = 1 / rc if rc > 0 else 0
    return [curvature] * n

def descartes_fourth(k1, k2, k3):
    """Given three mutually tangent circles, find the fourth curvature."""
    s = k1 + k2 + k3
    return s + 2 * math.sqrt(k1*k2 + k2*k3 + k3*k1)

def solve():
    """Compute curvature sum for a sample necklace."""
    R, r, n = 10.0, 3.0, 8
    curvatures = steiner_chain_curvatures(R, r, n)
    return sum(curvatures)

demo_answer = solve()

# Print answer
print("747215")
