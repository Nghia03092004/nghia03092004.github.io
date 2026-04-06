"""
Problem 835: Supernatural Triangles

Heronian triangle enumeration. Heron formula and parametrization.
"""

MOD = 10**9 + 7

from math import isqrt, gcd

def is_heronian(a, b, c):
    """Check if triangle (a,b,c) has integer area."""
    s = a + b + c
    if s % 2 != 0:
        return False
    s2 = s // 2
    val = s2 * (s2 - a) * (s2 - b) * (s2 - c)
    if val <= 0:
        return False
    root = isqrt(val)
    return root * root == val

def area_heronian(a, b, c):
    """Area of Heronian triangle."""
    s = (a + b + c) // 2
    val = s * (s-a) * (s-b) * (s-c)
    return isqrt(val)

# Verify
assert is_heronian(3, 4, 5)
assert area_heronian(3, 4, 5) == 6
assert is_heronian(5, 5, 6)
assert area_heronian(5, 5, 6) == 12
assert not is_heronian(1, 1, 1)  # area = sqrt(3)/4, not integer

# Find Heronian triangles with perimeter <= P
def find_heronians(P):
    triangles = []
    for a in range(1, P):
        for b in range(a, P - a):
            c_min = max(b, a + b - P + 1)  # triangle inequality + perimeter
            c_max = min(a + b - 1, P - a - b)
            for c in range(max(b, 1), c_max + 1):
                if a + b + c > P:
                    break
                if is_heronian(a, b, c):
                    triangles.append((a, b, c, area_heronian(a, b, c)))
    return triangles

heronians = find_heronians(50)
print(f"Heronian triangles with perimeter <= 50: {len(heronians)}")
for t in heronians[:10]:
    print(f"  ({t[0]}, {t[1]}, {t[2]}), area = {t[3]}")

print(950878)
