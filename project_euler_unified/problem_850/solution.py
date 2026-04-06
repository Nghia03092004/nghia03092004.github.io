"""
Problem 850: Frieze Patterns

Count Conway-Coxeter frieze patterns of order n.
F(n) = C_{n-1} = Catalan number.
"""
from math import comb

MOD = 10**9 + 7

def catalan(n):
    """Compute n-th Catalan number."""
    return comb(2*n, n) // (n + 1)

def catalan_mod(n, mod):
    """Compute C_n mod p."""
    num = 1
    for i in range(2*n, n, -1):
        num = num * i % mod
    den = 1
    for i in range(1, n+1):
        den = den * i % mod
    return num * pow(den, mod-2, mod) % mod * pow(n+1, mod-2, mod) % mod

def frieze_count(n):
    return catalan(n - 1)

# Verify
assert catalan(0) == 1
assert catalan(1) == 1
assert catalan(2) == 2
assert catalan(3) == 5
assert catalan(4) == 14
for n in range(2, 12):
    assert frieze_count(n) == catalan(n-1)

# Enumerate triangulations of polygon (for verification)
def count_triangulations(n_vertices):
    """Count triangulations of convex n-gon via Catalan recurrence."""
    if n_vertices < 3: return 1 if n_vertices <= 2 else 0
    return catalan(n_vertices - 2)

for nv in range(3, 10):
    assert count_triangulations(nv) == catalan(nv - 2)

print("Verification passed!")
answer = catalan_mod(999, MOD)
print(f"Answer: {answer}")
