"""
Problem 199: Iterative Circle Packing

Three unit circles inside an outer circle, iteratively packed for 10 iterations.
Find fraction of outer circle area not covered.
"""

import math

def main():
    # Outer circle radius
    R = 1.0 + 2.0 / math.sqrt(3.0)
    k_outer = -1.0 / R

    # Total area of packed circles (starting with 3 unit circles)
    total_area = 3.0 * math.pi

    # Initial gaps as curvature triples
    # 3 outer gaps: (k_outer, 1, 1)
    # 1 inner gap: (1, 1, 1)
    gaps = [(k_outer, 1.0, 1.0)] * 3 + [(1.0, 1.0, 1.0)]

    for iteration in range(10):
        new_gaps = []
        for a, b, c in gaps:
            # Descartes' circle theorem
            k_new = a + b + c + 2.0 * math.sqrt(a*b + b*c + c*a)
            total_area += math.pi / (k_new * k_new)
            new_gaps.append((a, b, k_new))
            new_gaps.append((a, c, k_new))
            new_gaps.append((b, c, k_new))
        gaps = new_gaps

    outer_area = math.pi * R * R
    fraction = 1.0 - total_area / outer_area

    print(f"{fraction:.8f}")
