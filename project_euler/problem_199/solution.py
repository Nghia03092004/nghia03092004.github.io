"""
Problem 199: Iterative Circle Packing

Use Descartes' theorem to expand the four initial gaps for ten iterations and
track the total covered area.
"""

import math


def solve(iterations=10):
    outer_radius = 1.0 + 2.0 / math.sqrt(3.0)
    outer_curvature = -1.0 / outer_radius
    covered_area = 3.0 * math.pi
    gaps = [(outer_curvature, 1.0, 1.0)] * 3 + [(1.0, 1.0, 1.0)]

    for _ in range(iterations):
        next_gaps = []
        for a, b, c in gaps:
            new_curvature = a + b + c + 2.0 * math.sqrt(a * b + b * c + c * a)
            covered_area += math.pi / (new_curvature * new_curvature)
            next_gaps.append((a, b, new_curvature))
            next_gaps.append((a, c, new_curvature))
            next_gaps.append((b, c, new_curvature))
        gaps = next_gaps

    outer_area = math.pi * outer_radius * outer_radius
    return 1.0 - covered_area / outer_area


if __name__ == "__main__":
    assert f"{solve(3):.8f}" == "0.06790342"
    print(f"{solve(10):.8f}")
