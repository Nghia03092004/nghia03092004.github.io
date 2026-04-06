import math

def solve():
    L_area = 1.0 - math.pi / 4.0
    threshold = 0.001  # 0.1%

    for n in range(1, 100000):
        s = 1.0 / n  # slope

        # Intersection of y = x/n with circle (x-1)^2 + (y-1)^2 = 1
        a = 1.0 + s * s
        b = -2.0 * (1.0 + s)
        c = 1.0
        disc = b * b - 4.0 * a * c
        x0 = (-b - math.sqrt(disc)) / (2.0 * a)
        y0 = x0 * s

        # Area of concave triangle
        # Part 1: triangle under line from 0 to x0
        area1 = x0 * y0 / 2.0

        # Part 2: area between circle bottom and x-axis from x0 to 1
        u0 = x0 - 1.0
        val_at_u0 = u0 - u0 * math.sqrt(1.0 - u0 * u0) / 2.0 - math.asin(u0) / 2.0
        area2 = 0.0 - val_at_u0

        concave_area = area1 + area2
        ratio = concave_area / L_area

        if ratio < threshold:
            print(n)
            return

solve()
