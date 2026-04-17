"""
Problem 903: Matrix Permanent Modulo a Prime

Let A be the n x n matrix with A[i,j] = (i*j) mod n, i,j in {1,...,n}.
Find perm(A) mod 10^9+7 for n = 12.

Key insight: Row n has A[n,j] = (n*j) mod n = 0 for all j, so perm(A) = 0.

Methods:
    1. Structural argument (zero row detection)
    2. Ryser's formula (general permanent computation)
    3. Brute-force via itertools.permutations (small n verification)
"""

from itertools import permutations
from math import gcd

MOD = 10**9 + 7

def solve_structural(n: int):
    """Detect if any row is all zeros => perm = 0."""
    for i in range(1, n + 1):
        if all((i * j) % n == 0 for j in range(1, n + 1)):
            return 0  # Row i is all zeros
    return None  # No zero row found; need computation

def permanent_ryser(A: list, mod: int = 0):
    """Compute permanent using Ryser's inclusion-exclusion formula.

    perm(A) = (-1)^n * sum_{S subset [n]} (-1)^|S| * prod_{i} sum_{j in S} A[i][j]
    Complexity: O(2^n * n).
    """
    n = len(A)
    result = 0
    for mask in range(1, 1 << n):
        bits = bin(mask).count("1")
        # Compute product of row sums restricted to columns in mask
        prod = 1
        for i in range(n):
            row_sum = 0
            for j in range(n):
                if mask & (1 << j):
                    row_sum += A[i][j]
            prod *= row_sum
            if prod == 0:
                break  # Early termination
        sign = (-1) ** (n - bits)
        result += sign * prod
    result *= (-1) ** n
    return result % mod if mod else result

def permanent_brute(A: list):
    """Compute permanent by definition: sum over all permutations."""
    n = len(A)
    total = 0
    for sigma in permutations(range(n)):
        prod = 1
        for i in range(n):
            prod *= A[i][sigma[i]]
            if prod == 0:
                break
        total += prod
    return total

# Build matrix and solve
N = 12
A = [[(i * j) % N for j in range(1, N + 1)] for i in range(1, N + 1)]

ans_structural = solve_structural(N)
assert ans_structural == 0, "Structural method failed"

ans_ryser = permanent_ryser(A) % MOD

for test_n in [3, 4, 5, 6, 7]:
    M = [[(i * j) % test_n for j in range(1, test_n + 1)] for i in range(1, test_n + 1)]
    p_ryser = permanent_ryser(M)
    p_brute = permanent_brute(M)
    assert p_ryser == p_brute, f"n={test_n}: Ryser={p_ryser}, brute={p_brute}"
    assert p_ryser == 0, f"n={test_n}: permanent should be 0, got {p_ryser}"

assert ans_ryser == 0

print(0)

# Permanents of the reduced matrix (i,j in {1..n-1}) for prime n
def permanent_reduced(n: int):
    """Permanent of the (n-1)x(n-1) matrix B[i,j] = (i*j) mod n, i,j in {1..n-1}."""
    B = [[(i * j) % n for j in range(1, n)] for i in range(1, n)]
    return permanent_ryser(B)
