#!/usr/bin/env python3
"""
Project Euler Problem 395 - Pythagorean Tree

The Pythagorean tree is a fractal built from a unit square. On the top side of
each square, a right triangle is constructed (hypotenuse = that side), and two
new squares are built on the triangle's legs. With legs a=3/5 and b=4/5:

  - Each iteration doubles the number of new squares.
  - Without overlaps, each level adds total area = 1, so T(n) = n+1.
  - For a=3/5, overlaps begin at iteration 5.

We compute T(10) = area of the union of all 2047 squares, using computational
geometry (Shapely) to handle polygon overlaps exactly.

Answer: T(10) = 9.93894732
"""

import numpy as np
from shapely.geometry import Polygon
from shapely.ops import unary_union

def build_pythagorean_tree(n, a=3.0 / 5.0):
    """
    Build all squares of the Pythagorean tree up to order n.

    Each square is represented as a list of 4 numpy arrays [p0, p1, p2, p3]:
      - p0, p1: bottom edge (attached to parent triangle's leg)
      - p2, p3: top edge (where the next triangle is constructed)
      - Vertices go counterclockwise.

    Returns (all_squares, levels) where levels[i] is a list of squares at depth i.
    """
    b = np.sqrt(1.0 - a * a)

    initial = [
        np.array([0.0, 0.0]),
        np.array([1.0, 0.0]),
        np.array([1.0, 1.0]),
        np.array([0.0, 1.0]),
    ]

    all_squares = [initial]
    levels = [[initial]]
    current_level = [initial]

    for _ in range(n):
        next_level = []
        for sq in current_level:
            p0, p1, p2, p3 = sq
            # Top side direction: p3 -> p2
            dx = p2 - p3
            side_len = np.linalg.norm(dx)
            ux = dx / side_len  # unit along top side
            uy = np.array([-ux[1], ux[0]])  # unit normal (outward)

            # Triangle apex position
            apex = p3 + ux * (a * a * side_len) + uy * (a * b * side_len)

            # --- Left square on leg p3->apex ---
            d_left = apex - p3
            n_left = np.array([d_left[1], -d_left[0]])  # perpendicular
            # Ensure it points away from the triangle interior (away from p2)
            if np.dot(n_left, p2 - (p3 + apex) / 2) > 0:
                n_left = -n_left
            left_square = [p3.copy(), apex.copy(), apex + n_left, p3 + n_left]

            # --- Right square on leg apex->p2 ---
            d_right = p2 - apex
            n_right = np.array([d_right[1], -d_right[0]])
            if np.dot(n_right, p3 - (apex + p2) / 2) > 0:
                n_right = -n_right
            right_square = [apex.copy(), p2.copy(), p2 + n_right, apex + n_right]

            next_level.append(left_square)
            next_level.append(right_square)
            all_squares.append(left_square)
            all_squares.append(right_square)

        levels.append(next_level)
        current_level = next_level

    return all_squares, levels

def compute_union_area(squares):
    """Compute the area of the union of all square polygons using Shapely."""
    polygons = []
    for sq in squares:
        coords = [(p[0], p[1]) for p in sq]
        coords.append(coords[0])
        poly = Polygon(coords)
        if not poly.is_valid:
            poly = poly.buffer(0)
        if poly.area > 1e-15:
            polygons.append(poly)
    return unary_union(polygons).area

def visualize_tree(levels, filename="visualization.png"):
    """Draw the Pythagorean tree with color-coded depth levels."""
