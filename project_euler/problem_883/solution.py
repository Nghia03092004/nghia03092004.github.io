"""
Problem 883: Remarkable Representations
Sum of squares of irreducible representation dimensions equals group order.
"""

from math import gcd
from itertools import product

def verify_sum_of_squares(group_order, dims):
    """Verify sum(d^2) = |G| and each d divides |G|."""
    s = sum(d * d for d in dims)
    divides = all(group_order % d == 0 for d in dims)
    return s == group_order and divides

def cyclic_dims(n):
    """Cyclic group Z/nZ: n representations, all 1-dimensional."""
    return [1] * n

def symmetric_dims(n):
    """Known irreducible dimensions for S_n."""
    known = {
        1: [1], 2: [1, 1], 3: [1, 1, 2],
        4: [1, 1, 2, 3, 3], 5: [1, 1, 4, 4, 5, 5, 6]
    }
    return known.get(n, None)

def dihedral_dims(n):
    """Dihedral group D_n of order 2n."""
    if n % 2 == 1:  # odd n
        return [1, 1] + [2] * ((n - 1) // 2)
    else:  # even n
        return [1, 1, 1, 1] + [2] * ((n - 2) // 2)

def factorials():
    return {1: 1, 2: 2, 3: 6, 4: 24, 5: 120, 6: 720}

def find_dim_partitions(n, max_d=None):
    """Find all ways to write n as sum of squares d_i^2 where d_i | n."""
    if max_d is None:
        max_d = n
    divisors = [d for d in range(1, n + 1) if n % d == 0 and d <= max_d]
    results = []
    def backtrack(remaining, min_d, current):
        if remaining == 0:
            results.append(tuple(current))
            return
        for d in divisors:
            if d >= min_d and d * d <= remaining:
                backtrack(remaining - d * d, d, current + [d])
    backtrack(n, 1, [])
    return results

# --- Verification ---
print("=== Sum of Squares Verification ===")
groups = [
    ("Z/4", 4, cyclic_dims(4)),
    ("Z/6", 6, cyclic_dims(6)),
    ("S_3", 6, [1, 1, 2]),
    ("D_4", 8, dihedral_dims(4)),
    ("Q_8", 8, [1, 1, 1, 1, 2]),
    ("A_4", 12, [1, 1, 1, 3]),
    ("S_4", 24, [1, 1, 2, 3, 3]),
]
for name, order, dims in groups:
    ok = verify_sum_of_squares(order, dims)
    s = sum(d*d for d in dims)
    print(f"  {name:>4}: |G|={order:>3}, dims={dims}, sum={s}, {'OK' if ok else 'FAIL'}")
    assert ok

print("\n=== Dihedral Groups ===")
for n in range(3, 11):
    dims = dihedral_dims(n)
    order = 2 * n
    ok = verify_sum_of_squares(order, dims)
    print(f"  D_{n}: |D|={order}, dims={dims}, sum={sum(d*d for d in dims)}, {'OK' if ok else 'FAIL'}")
    assert ok

print("\n=== Dimension Partitions of 12 ===")
parts = find_dim_partitions(12)
for p in parts:
    print(f"  {p}: sum of squares = {sum(d*d for d in p)}")

answer = sum(d*d for d in [1, 1, 2, 3, 3])
print(f"\nAnswer: S_4 dimension sum = {answer} = |S_4| = 24")

# --- 4-Panel Visualization ---
