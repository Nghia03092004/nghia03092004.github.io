"""
Problem 912: Polynomial Roots over Finite Fields

For p = 997 and f(x) = x^3 + 2x + 1 over F_p, count the roots.

Key results:
    - Lagrange: deg d polynomial has at most d roots
    - Discriminant Delta = -4p^3 - 27q^2 for x^3 + px + q
    - Delta QR => 0 or 3 roots; Delta QNR => 1 root

Methods:
    1. Brute-force evaluation over all x in F_p
    2. Horner's method evaluation
    3. Discriminant-based analysis
"""

def find_roots_brute(p: int, coeffs: list) -> list:
    """Find all roots of polynomial in F_p by exhaustive evaluation.

    coeffs = [a_d, ..., a_1, a_0] for a_d*x^d + ... + a_0.
    """
    roots = []
    for x in range(p):
        val = 0
        for c in coeffs:
            val = (val * x + c) % p
        if val == 0:
            roots.append(x)
    return roots

def poly_gcd_roots(p: int, coeffs: list):
    """Count roots using gcd(f(x), x^p - x) in F_p[x].

    Every element of F_p is a root of x^p - x (Fermat).
    So gcd(f, x^p - x) has exactly the common roots = roots of f in F_p.
    """
    # For small p, brute force is simpler. This is the algebraic approach.
    # Implement polynomial operations mod p
    pass  # Using brute force instead for p = 997

# Discriminant analysis
def legendre_symbol(a: int, p: int):
    """Compute Legendre symbol (a/p) via Euler's criterion."""
    a = a % p
    if a == 0:
        return 0
    result = pow(a, (p - 1) // 2, p)
    return result if result <= 1 else -1

def discriminant_cubic(p_coeff: int, q_coeff: int):
    """Discriminant of x^3 + px + q is -4p^3 - 27q^2."""
    return -4 * p_coeff ** 3 - 27 * q_coeff ** 2

# Solve
P = 997

# f(x) = x^3 + 2x + 1, coefficients [1, 0, 2, 1]
roots = find_roots_brute(P, [1, 0, 2, 1])
count = len(roots)

# Discriminant analysis
delta = discriminant_cubic(2, 1)  # -4*8 - 27 = -59
leg = legendre_symbol(delta, P)

# Verify: roots found by brute force
for r in roots:
    assert (r ** 3 + 2 * r + 1) % P == 0, f"False root: {r}"

# Verify no other roots
for x in range(P):
    if (x ** 3 + 2 * x + 1) % P == 0:
        assert x in roots

# Check root count against theory
# For various cubics over F_997, verify Lagrange bound
for test_poly in [[1, 0, 0, 1], [1, 1, 1, 1], [1, 0, -1, 0]]:
    test_roots = find_roots_brute(P, test_poly)
    assert len(test_roots) <= 3  # degree 3 => at most 3 roots

print(count)
