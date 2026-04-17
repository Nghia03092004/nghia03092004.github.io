"""
Problem 317: Firecracker

A firecracker at height h0 = 100m ejects fragments at v0 = 20 m/s in all directions.
Find the volume of the reachable region (no air resistance, g = 9.81 m/s^2).

The envelope of all parabolic trajectories forms a paraboloid of revolution:
    z_max(r) = Z - g*r^2 / (2*v0^2)
where Z = h0 + v0^2 / (2*g) is the maximum height.

Volume = pi * v0^2 * Z^2 / g
"""

import math

# Optional visualization
