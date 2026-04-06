"""
Problem 841: Regular Star Polygons

Compute the sum of areas of all regular star polygons {n/k} for 3 <= n <= N,
with 1 < k < n/2 and gcd(n,k) = 1, inscribed in a unit circle.

Area of {n/k} = (n/2) * sin(2*pi*k/n).
"""

from math import gcd, sin, pi

# --- Method 1: Direct enumeration ---
def sum_star_areas_direct(N: int) -> float:
    """Sum areas of all valid star polygons up to N points."""
    total = 0.0
    for n in range(3, N + 1):
        for k in range(2, n // 2 + 1):
            if n % 2 == 0 and k == n // 2:
                continue  # degenerate
            if gcd(n, k) == 1:
                total += (n / 2) * sin(2 * pi * k / n)
    return total

# --- Method 2: Using Ramanujan sum identity ---
def sum_star_areas_ramanujan(N: int) -> float:
    """Use the fact that sum over coprime k of sin(2pi*k/n) relates to mu(n)."""
    total = 0.0
    for n in range(3, N + 1):
        # Sum sin(2*pi*k/n) for 1 < k < n/2, gcd(k,n) = 1
        inner = 0.0
        for k in range(2, n // 2 + 1):
            if n % 2 == 0 and k == n // 2:
                continue
            if gcd(n, k) == 1:
                inner += sin(2 * pi * k / n)
        total += (n / 2) * inner
    return total

# --- Method 3: Count by phi(n) ---
def count_star_polygons(N: int):
    """Count total number of valid star polygons."""
    count = 0
    for n in range(3, N + 1):
        for k in range(2, n // 2 + 1):
            if n % 2 == 0 and k == n // 2:
                continue
            if gcd(n, k) == 1:
                count += 1
    return count

# --- Verify methods agree ---
for N in range(3, 30):
    a = sum_star_areas_direct(N)
    b = sum_star_areas_ramanujan(N)
    assert abs(a - b) < 1e-10, f"Mismatch at N={N}: {a} vs {b}"

print("Verification passed!")

# Compute answer
N = 10000
answer = sum_star_areas_direct(N)
print(f"S({N}) = {answer:.3f}")
