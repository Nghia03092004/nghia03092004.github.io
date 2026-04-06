"""
Problem 410: Circle and Tangent Line

Given a circle centered at the origin with radius r and an external point P=(a,0),
compute tangent lines, tangent points, tangent length, and the enclosed area.
The helper data returned by create_visualization can be consumed by an external plotter.
"""

from __future__ import annotations

import math
import sys
from typing import Dict, Optional, Sequence, Tuple

def tangent_length(a: float, r: float) -> float:
    """Distance from external point (a,0) to tangent point on circle of radius r."""
    assert a > r > 0, "Point must be outside the circle"
    return math.sqrt(a**2 - r**2)

def tangent_half_angle(a: float, r: float) -> float:
    """Half-angle (in radians) subtended at P by the two tangent lines."""
    assert a > r > 0
    return math.asin(r / a)

def tangent_points(a: float, r: float):
    """Coordinates of the two tangent points on the circle."""
    assert a > r > 0
    tx = r**2 / a
    ty = r * math.sqrt(a**2 - r**2) / a
    return (tx, ty), (tx, -ty)

def tangent_line_coefficients(
    a: float, r: float
) -> Tuple[Tuple[float, float, float], Tuple[float, float, float]]:
    """
    Return (A, B, C) for each tangent line Ax + By = C.
    Line 1: r*x + sqrt(a^2-r^2)*y = r*a
    Line 2: r*x - sqrt(a^2-r^2)*y = r*a
    """
    assert a > r > 0
    L = math.sqrt(a**2 - r**2)
    return (r, L, r * a), (r, -L, r * a)

def enclosed_area(a: float, r: float) -> float:
    """
    Area between the two tangent lines and the minor arc of the circle.
    Equals L*r - r^2 * arcsin(r/a), where L = sqrt(a^2 - r^2).
    """
    assert a > r > 0
    L = math.sqrt(a**2 - r**2)
    alpha = math.asin(r / a)
    return L * r - r**2 * alpha

def power_of_point(a: float, r: float) -> float:
    """Power of the point P=(a,0) with respect to the circle x^2+y^2=r^2."""
    return a**2 - r**2

def print_solution(a: float, r: float) -> None:
    """Print all computed quantities for given parameters."""
    print(f"Circle: center (0,0), radius r = {r}")
    print(f"External point: P = ({a}, 0)")
    print(f"{'='*50}")

    L = tangent_length(a, r)
    print(f"Tangent length:        L = {L:.6f}")

    alpha = tangent_half_angle(a, r)
    print(f"Half-angle:            alpha = {math.degrees(alpha):.4f} deg  ({alpha:.6f} rad)")

    t1, t2 = tangent_points(a, r)
    print(f"Tangent point T1:      ({t1[0]:.6f}, {t1[1]:.6f})")
    print(f"Tangent point T2:      ({t2[0]:.6f}, {t2[1]:.6f})")

    (a1, b1, c1), (a2, b2, c2) = tangent_line_coefficients(a, r)
    print(f"Tangent line 1:        {a1:.4f}*x + {b1:.4f}*y = {c1:.4f}")
    print(f"Tangent line 2:        {a2:.4f}*x + {b2:.4f}*y = {c2:.4f}")

    A = enclosed_area(a, r)
    print(f"Enclosed area:         A = {A:.6f}")

    pw = power_of_point(a, r)
    print(f"Power of point:        {pw:.6f}  (= L^2 = {L**2:.6f})")

    # Verify tangent points lie on the circle
    for name, pt in [("T1", t1), ("T2", t2)]:
        dist = math.sqrt(pt[0]**2 + pt[1]**2)
        print(f"Verification |O{name}|:   {dist:.6f}  (should be {r})")

    # Verify tangent is perpendicular to radius at tangent point
    for name, pt in [("T1", t1), ("T2", t2)]:
        # OT vector: (tx, ty), PT vector: (tx - a, ty)
        dot = pt[0] * (pt[0] - a) + pt[1] * pt[1]
        print(f"Verification OT.PT ({name}): {dot:.6e}  (should be 0)")

def create_visualization(
    a: float, r: float, filename: str = "visualization.png"
) -> Dict[str, object]:
    """Return geometry data that an external plotting tool can consume."""
    t1, t2 = tangent_points(a, r)
    return {
        "circle_center": (0.0, 0.0),
        "radius": r,
        "external_point": (a, 0.0),
        "tangent_points": (t1, t2),
        "tangent_lines": tangent_line_coefficients(a, r),
        "suggested_output": filename,
    }


def parse_args(argv: Sequence[str]) -> Tuple[float, float]:
    if not argv:
        return 3.0, 1.0
    if len(argv) != 2:
        raise SystemExit("usage: python3 solution.py [a r]")
    return float(argv[0]), float(argv[1])


def main(argv: Optional[Sequence[str]] = None) -> None:
    a, r = parse_args(sys.argv[1:] if argv is None else argv)
    print_solution(a, r)


if __name__ == "__main__":
    main()
