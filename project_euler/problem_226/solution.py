"""
Problem 226: A Scoop of Blancmange

Find the area under the Blancmange curve enclosed by the circle
(x - 1/4)^2 + (y - 1/2)^2 = 1/16, to 8 decimal places.
"""

import math

def s(x):
    """Triangle wave: distance to nearest integer."""
    return abs(x - round(x))

def blanc(x, terms=60):
    """Blancmange (Takagi) curve value at x."""
    result = 0.0
    pow2 = 1.0
    for _ in range(terms):
        result += s(pow2 * x) / pow2
        pow2 *= 2.0
    return result

def on_circle(x):
    """Returns (x-1/4)^2 + (blanc(x)-1/2)^2 - 1/16. Zero on circle."""
    b = blanc(x)
    return (x - 0.25)**2 + (b - 0.5)**2 - 1.0/16

def circle_lower(x):
    """Lower arc of circle: y = 1/2 - sqrt(1/16 - (x-1/4)^2)."""
    val = 1.0/16 - (x - 0.25)**2
    if val < 0:
        return 0.5
    return 0.5 - math.sqrt(val)

def bisect_root(f, a, b, tol=1e-14):
    """Find root of f in [a, b] using bisection."""
    fa, fb = f(a), f(b)
    if fa * fb > 0:
        return a
    for _ in range(100):
        mid = (a + b) / 2
        fm = f(mid)
        if abs(b - a) < tol:
            return mid
        if fa * fm <= 0:
            b, fb = mid, fm
        else:
            a, fa = mid, fm
    return (a + b) / 2

def integrand(x):
    """blanc(x) - circle_lower(x)"""
    return blanc(x) - circle_lower(x)

def simpsons(f, a, b, n):
    """Simpson's rule with n subdivisions (n must be even)."""
    h = (b - a) / n
    result = f(a) + f(b)
    for i in range(1, n):
        x = a + i * h
        result += (2 if i % 2 == 0 else 4) * f(x)
    return result * h / 3

# Find intersection point
x1 = bisect_root(on_circle, 0.05, 0.1)
x2 = 0.5

# Compute area using Simpson's rule with fine grid
N = 100000  # 100K subdivisions for reasonable Python speed
area = simpsons(integrand, x1, x2, N)

print(f"{area:.8f}")

# Optional visualization
