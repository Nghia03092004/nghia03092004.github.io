"""
Problem 264: Triangle Centres

Find all triangles ABC with:
- All vertices have integer coordinates on circle x^2+y^2 = n
- Circumcenter at origin O(0,0)
- Orthocenter at H(5,0), which means A+B+C = (5,0)
- Perimeter <= 100000
- Non-degenerate (positive area)

Sum all such perimeters, rounded to 4 decimal places.

Algorithm:
For each lattice point A = (a1, a2) with n = a1^2+a2^2, the orthocenter
condition gives a LINEAR constraint on B = (b1, b2):
  (2*a1 - 10)*b1 + 2*a2*b2 = 10*a1 - 25 - n

Combined with b1^2 + b2^2 = n (same circle), this is a line-circle
intersection with at most 2 solutions.

The discriminant simplifies to D = n*(L^2+4*a2^2) - R^2, where
L = 2*a1-10 and R = 10*a1-25-n. D must be a non-negative perfect square.

Each unordered triangle {A,B,C} is counted 6 times (3! orderings).

There are 155 valid triangles. All have circumradius <= 20000.

Answer: 2816417.1055
"""

import math

def solve():
    MAX_PERIMETER = 100000.0
    R_MAX = 20000  # No triangles found beyond R=20000

    total_perimeter = 0.0
    triangle_count = 0

    for a1 in range(-R_MAX, R_MAX + 1):
        L = 2 * a1 - 10
        max_a2_sq = R_MAX * R_MAX - a1 * a1
        if max_a2_sq < 0:
            continue
        max_a2 = int(math.isqrt(max_a2_sq))

        for a2 in range(1, max_a2 + 1):
            n = a1 * a1 + a2 * a2
            R_val = 10 * a1 - 25 - n

            # D must be a non-negative perfect square
            D = n * (L * L + 4 * a2 * a2) - R_val * R_val
            if D < 0:
                continue

            sq_D = int(math.isqrt(D))
            if sq_D * sq_D != D:
                continue

            # Process both signs of a2
            for a2_sign in (-1, 1):
                a2v = a2 * a2_sign
                M = 2 * a2v

                A_c = M * M + L * L
                B_c = -2 * R_val * L
                sq_disc = 4 * a2 * sq_D

                for sign in (-1, 1):
                    num = -B_c + sign * sq_disc
                    denom = 2 * A_c
                    if denom == 0:
                        continue
                    if num % denom != 0:
                        continue
                    b1 = num // denom

                    num2 = R_val - L * b1
                    if num2 % M != 0:
                        continue
                    b2 = num2 // M

                    if b1 * b1 + b2 * b2 != n:
                        continue

                    c1 = 5 - a1 - b1
                    c2 = -a2v - b2
                    if c1 * c1 + c2 * c2 != n:
                        continue

                    cross = (b1 - a1) * (c2 - a2v) - (b2 - a2v) * (c1 - a1)
                    if cross == 0:
                        continue

                    dAB = math.sqrt((a1 - b1) ** 2 + (a2v - b2) ** 2)
                    dBC = math.sqrt((b1 - c1) ** 2 + (b2 - c2) ** 2)
                    dCA = math.sqrt((c1 - a1) ** 2 + (c2 - a2v) ** 2)
                    peri = dAB + dBC + dCA

                    if peri <= MAX_PERIMETER:
                        total_perimeter += peri
                        triangle_count += 1

        # Handle a2 = 0
        n = a1 * a1
        if n > 0:
            R_val = 10 * a1 - 25 - n
            if L == 0:
                # a1=5, n=25
                for bb1 in range(-5, 6):
                    bb2_sq = 25 - bb1 * bb1
                    if bb2_sq < 0:
                        continue
                    bb2 = int(math.isqrt(bb2_sq))
                    if bb2 * bb2 != bb2_sq:
                        continue
                    for s in ([-1, 1] if bb2 > 0 else [1]):
                        b2v = s * bb2
                        c1v, c2v = -bb1, -b2v
                        if c1v * c1v + c2v * c2v != 25:
                            continue
                        cross = (bb1 - 5) * c2v - b2v * (c1v - 5)
                        if cross == 0:
                            continue
                        d1 = math.sqrt((5 - bb1) ** 2 + b2v ** 2)
                        d2 = math.sqrt((bb1 - c1v) ** 2 + (b2v - c2v) ** 2)
                        d3 = math.sqrt((c1v - 5) ** 2 + c2v ** 2)
                        peri = d1 + d2 + d3
                        if peri <= MAX_PERIMETER:
                            total_perimeter += peri
                            triangle_count += 1
            elif R_val % L == 0:
                b1 = R_val // L
                b2_sq = n - b1 * b1
                if b2_sq >= 0:
                    b2 = int(math.isqrt(b2_sq))
                    if b2 * b2 == b2_sq:
                        for s in ([-1, 1] if b2 > 0 else [1]):
                            b2v = s * b2
                            c1, c2 = 5 - a1 - b1, -b2v
                            if c1 * c1 + c2 * c2 != n:
                                continue
                            cross = (b1 - a1) * c2 - b2v * (c1 - a1)
                            if cross == 0:
                                continue
                            d1 = math.sqrt((a1 - b1) ** 2 + b2v ** 2)
                            d2 = math.sqrt((b1 - c1) ** 2 + (b2v - c2) ** 2)
                            d3 = math.sqrt((c1 - a1) ** 2 + c2 ** 2)
                            peri = d1 + d2 + d3
                            if peri <= MAX_PERIMETER:
                                total_perimeter += peri
                                triangle_count += 1

        if a1 % 5000 == 0:
            print(f"a1={a1}, triangles={triangle_count//6}, sum={total_perimeter/6:.4f}", flush=True)

    total_perimeter /= 6.0
    triangle_count //= 6
    print(f"{total_perimeter:.4f}")

if __name__ == "__main__":
    solve()
