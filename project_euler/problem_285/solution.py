"""
Project Euler Problem 285: Pythagorean Odds

Albert picks a,b uniformly in [0,1]. Computes sqrt((ka+1)^2+(kb+1)^2), rounds to nearest
integer. Scores k if result = k, else 0. Find expected total for k=1..100000, to 5 decimals.

Winning condition: (k-1/2)^2 <= (ka+1)^2 + (kb+1)^2 < (k+1/2)^2

Substituting x=a+1/k, y=b+1/k: circle centered at origin, x in [1/k, 1+1/k], y same.
Inner radius r1 = (k-1/2)/k, outer radius r2 = (k+1/2)/k.

Area of {x>=c, y>=c, x^2+y^2 <= r^2} where c=1/k:
For r^2 >= 2c^2: integral_c^sqrt(r^2-c^2) (sqrt(r^2-x^2) - c) dx
= [x*sqrt(r^2-x^2)/2 + r^2/2 * arcsin(x/r)] from c to sqrt(r^2-c^2) - c*(sqrt(r^2-c^2)-c)

Uses mpmath for sufficient precision.
"""

from mpmath import mp, mpf, sqrt, asin

mp.dps = 25

def circle_quad_area(r, c):
    """Area of {x >= c, y >= c, x^2+y^2 <= r^2}."""
    if r * r < 2 * c * c:
        return mpf(0)
    x_max = sqrt(r * r - c * c)

    def F(x):
        return x * sqrt(r * r - x * x) / 2 + r * r / 2 * asin(x / r)

    return F(x_max) - F(c) - c * (x_max - c)

total = mpf(0)
for k in range(1, 100001):
    c = mpf(1) / k
    r1 = (mpf(2) * k - 1) / (2 * k)
    r2 = (mpf(2) * k + 1) / (2 * k)
    total += k * (circle_quad_area(r2, c) - circle_quad_area(r1, c))

print(f"{float(total):.5f}")
