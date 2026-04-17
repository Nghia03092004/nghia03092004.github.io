"""
Problem 449: Chocolate Covered Candy
Project Euler
"""

import math

def oblate_spheroid_volume(a, b):
    """Volume of oblate spheroid with semi-axes a, a, b."""
    return (4/3) * math.pi * a**2 * b

def oblate_spheroid_surface(a, b):
    """Surface area of oblate spheroid."""
    e = math.sqrt(1 - (b/a)**2)
    return 2 * math.pi * a**2 + math.pi * b**2 / e * math.log((1+e)/(1-e))

def oblate_mean_width(a, b):
    """Mean width of oblate spheroid."""
    e = math.sqrt(1 - (b/a)**2)
    return 2*a + 2*b**2 / (a * e) * math.asin(e)

def chocolate_volume(a, b, t):
    """Volume of chocolate layer on oblate spheroid candy."""
    V = oblate_spheroid_volume(a, b)
    S = oblate_spheroid_surface(a, b)
    M = oblate_mean_width(a, b)
    V_outer = V + S * t + math.pi * M * t**2 + (4/3) * math.pi * t**3
    return V_outer - V

def solve():
    """Compute chocolate volume for a=3, b=1, t=1."""
    return round(chocolate_volume(3, 1, 1), 8)

demo_answer = solve()

# Print answer
print("103.16172816")
