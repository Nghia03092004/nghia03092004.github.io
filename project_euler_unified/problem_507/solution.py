"""
Problem 507: Shortest Lattice Vector
Find shortest non-zero vectors in lattices defined by quadratic forms.
Uses Gauss reduction for 2D lattices and LLL for higher dimensions.
"""

import numpy as np
from math import gcd, isqrt

def gauss_reduce(a: int, b: int, c: int):
    """
    Gauss reduction of a positive-definite binary quadratic form ax^2 + bxy + cy^2.
    Returns the reduced form (a', b', c') with a' <= c' and |b'| <= a'.
    The minimum non-zero value represented is a'.
    """
    while True:
        # Ensure a <= c
        if a > c:
            a, c = c, a
        # Size-reduce b
        if abs(b) > a:
            # Replace b by b - 2*round(b/(2a))*a
            k = round(b / (2 * a))
            c = c - k * b + k * k * a
            b = b - 2 * k * a
        else:
            break
    if a > c:
        a, c = c, a
    return (a, b, c)

def shortest_vector_2d(a: int, b: int, c: int):
    """
    Find the minimum non-zero value of the quadratic form ax^2 + bxy + cy^2.
    Uses Gauss reduction.
    """
    a_red, b_red, c_red = gauss_reduce(a, b, c)
    return a_red

def lll_reduce(basis: np.ndarray, delta: float = 0.75) -> np.ndarray:
    """
    LLL lattice basis reduction algorithm.
    basis: matrix where rows are basis vectors.
    Returns the reduced basis.
    """
    n = basis.shape[0]
    B = basis.astype(float).copy()

    def gram_schmidt():
        ortho = np.zeros_like(B)
        mu = np.zeros((n, n))
        ortho[0] = B[0].copy()
        for i in range(1, n):
            ortho[i] = B[i].copy()
            for j in range(i):
                if np.dot(ortho[j], ortho[j]) < 1e-10:
                    mu[i][j] = 0
                else:
                    mu[i][j] = np.dot(B[i], ortho[j]) / np.dot(ortho[j], ortho[j])
                ortho[i] -= mu[i][j] * ortho[j]
        return ortho, mu

    k = 1
    while k < n:
        ortho, mu = gram_schmidt()

        # Size reduction
        for j in range(k - 1, -1, -1):
            if abs(mu[k][j]) > 0.5:
                B[k] -= round(mu[k][j]) * B[j]
                ortho, mu = gram_schmidt()

        # Lovasz condition
        norm_k = np.dot(ortho[k], ortho[k])
        norm_km1 = np.dot(ortho[k-1], ortho[k-1])

        if norm_k >= (delta - mu[k][k-1]**2) * norm_km1:
            k += 1
        else:
            B[[k, k-1]] = B[[k-1, k]]
            k = max(k - 1, 1)

    return B

def solve_for_range(N: int) -> list:
    """
    Compute shortest vectors for a family of quadratic forms.
    Form: Q_n(x,y) = n*x^2 + x*y + n*y^2 for n = 1..N.
    """
    results = []
    for n in range(1, N + 1):
        sv = shortest_vector_2d(n, 1, n)
        results.append((n, sv))
    return results

def solve_brute_force(a: int, b: int, c: int, bound: int = 100):
    """Brute-force minimum of ax^2 + bxy + cy^2 for verification."""
    min_val = float('inf')
    for x in range(-bound, bound + 1):
        for y in range(-bound, bound + 1):
            if x == 0 and y == 0:
                continue
            val = a * x * x + b * x * y + c * y * y
            if val > 0:
                min_val = min(min_val, val)
    return min_val

# Verify Gauss reduction against brute force
print("Verification:")
for a, b, c in [(3, 2, 5), (7, 3, 4), (10, 6, 10), (1, 1, 1)]:
    sv = shortest_vector_2d(a, b, c)
    bf = solve_brute_force(a, b, c)
    status = "OK" if sv == bf else "MISMATCH"
    print(f"  Q = {a}x^2 + {b}xy + {c}y^2: Gauss={sv}, BF={bf} [{status}]")

# Compute for the family
N = 200
results = solve_for_range(N)
total = sum(sv for _, sv in results)
print(f"\nSum of shortest vectors for n=1..{N}: {total}")

# LLL example in 3D
basis_3d = np.array([[1, 0, 0], [0, 1, 0], [3, 4, 5]], dtype=float)
reduced = lll_reduce(basis_3d)
print(f"\n3D LLL reduction:")
print(f"  Original basis:\n{basis_3d}")
print(f"  Reduced basis:\n{reduced}")
print(f"  Shortest vector length: {np.linalg.norm(reduced[0]):.6f}")
