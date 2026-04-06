"""
Problem 721: High Powers of Irrational Numbers

Compute G(N) = sum_{a=1}^{N} f(a, a^2) mod 999999937
where f(a, n) = floor((ceil(sqrt(a)) + sqrt(a))^n)
and a ranges over non-perfect-squares.

Key insight: alpha = ceil(sqrt(a)) + sqrt(a), beta = ceil(sqrt(a)) - sqrt(a)
satisfy a quadratic, so alpha^n + beta^n is an integer (via linear recurrence).
Since 0 < beta < 1, f(a,n) = alpha^n + beta^n - 1.
"""

from math import isqrt

MOD = 999_999_937

def mat_mul(A, B, mod):
    """Multiply two 2x2 matrices modulo mod."""
    return [
        [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
         (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
        [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
         (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod],
    ]

def mat_pow(M, n, mod):
    """Compute M^n mod p for a 2x2 matrix."""
    result = [[1, 0], [0, 1]]  # identity
    base = [row[:] for row in M]
    while n > 0:
        if n & 1:
            result = mat_mul(result, base, mod)
        base = mat_mul(base, base, mod)
        n >>= 1
    return result

def T_n(m, c, n, mod):
    """Compute T_n = alpha^n + beta^n mod p using matrix exponentiation.
    Recurrence: T_k = 2m * T_{k-1} - c * T_{k-2}, T_0=2, T_1=2m.
    Here c = m^2 - a.
    """
    if n == 0:
        return 2 % mod
    if n == 1:
        return (2 * m) % mod
    M = [[(2*m) % mod, (-c) % mod], [1, 0]]
    R = mat_pow(M, n - 1, mod)
    # T_n = R[0][0]*T_1 + R[0][1]*T_0
    return (R[0][0] * (2*m) + R[0][1] * 2) % mod

def is_perfect_square(a):
    s = isqrt(a)
    return s * s == a

# --- Method 1: Direct computation for small N ---
def solve_small(N):
    """Compute G(N) mod p for small N (brute force verification)."""
    total = 0
    for a in range(1, N + 1):
        if is_perfect_square(a):
            continue
        s = isqrt(a)
        m = s + 1  # ceil(sqrt(a))
        c = m * m - a
        n = a * a
        val = T_n(m, c, n, MOD)
        total = (total + val - 1) % MOD
    return total

# --- Method 2: Same algorithm, for full N ---
def solve(N):
    """Compute G(N) mod p."""
    return solve_small(N)

# --- Verification ---
# f(5, 2): m=3, c=4, T_2 = 28, f=27
m, c = 3, 4
assert T_n(m, c, 2, MOD) == 28
assert T_n(m, c, 2, MOD) - 1 == 27  # f(5,2) = 27

# f(5, 5): T_5 = 3936, f=3935
assert T_n(m, c, 5, MOD) == 3936
assert T_n(m, c, 5, MOD) - 1 == 3935  # f(5,5) = 3935

# G(1000) = 163861845
result_1000 = solve(1000)
assert result_1000 == 163861845, f"G(1000) = {result_1000}, expected 163861845"

print(f"G(1000) = {result_1000}")

# Full answer would be solve(5000000) -- takes a few minutes
# ans = solve(5000000)
# print(ans)
