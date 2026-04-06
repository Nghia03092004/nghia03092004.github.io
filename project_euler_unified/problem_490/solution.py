"""
Problem 490: Jumping Frog
A frog jumps on stepping stones 0..n, jumping 1, 2, or 3 steps forward.
Count the number of distinct paths from 0 to n.
Uses tribonacci recurrence and matrix exponentiation.
"""

def solve_dp(n: int, mod: int = 0):
    """Count paths using DP (tribonacci recurrence). O(n) time."""
    if n == 0:
        return 1
    if n == 1:
        return 1
    if n == 2:
        return 2

    a, b, c = 1, 1, 2  # f(0), f(1), f(2)
    for _ in range(3, n + 1):
        a, b, c = b, c, (a + b + c) if mod == 0 else (a + b + c) % mod
    return c

def mat_mult(A: list, B: list, mod: int) -> list:
    """Multiply two 3x3 matrices modulo mod."""
    n = len(A)
    C = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            s = 0
            for k in range(n):
                s += A[i][k] * B[k][j]
            C[i][j] = s % mod if mod else s
    return C

def mat_pow(M: list, p: int, mod: int) -> list:
    """Compute M^p mod mod using exponentiation by squaring."""
    n = len(M)
    # Identity matrix
    result = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    base = [row[:] for row in M]
    while p > 0:
        if p & 1:
            result = mat_mult(result, base, mod)
        base = mat_mult(base, base, mod)
        p >>= 1
    return result

def solve_matrix(n: int, mod: int = 10**9 + 7):
    """Count paths using matrix exponentiation. O(log n) time."""
    if n == 0:
        return 1
    if n == 1:
        return 1
    if n == 2:
        return 2 % mod

    A = [[1, 1, 1],
         [1, 0, 0],
         [0, 1, 0]]

    An = mat_pow(A, n, mod)
    # f(n) = An[0][0]*f(0) + An[0][1]*f(-1) + An[0][2]*f(-2)
    # But we use: (f(n), f(n-1), f(n-2)) = A^(n-2) * (f(2), f(1), f(0))
    An2 = mat_pow(A, n - 2, mod)
    result = (An2[0][0] * 2 + An2[0][1] * 1 + An2[0][2] * 1) % mod
    return result

def solve_dp_with_forbidden(n: int, forbidden: set):
    """Count paths avoiding forbidden stones."""
    f = [0] * (n + 1)
    f[0] = 1
    for k in range(1, n + 1):
        if k in forbidden:
            f[k] = 0
        else:
            f[k] = sum(f[k - j] for j in [1, 2, 3] if k - j >= 0)
    return f[n]

# Verify: DP vs matrix
mod = 10**9 + 7
for n in range(20):
    dp_val = solve_dp(n, mod)
    mat_val = solve_matrix(n, mod)
    assert dp_val == mat_val, f"Mismatch at n={n}: {dp_val} vs {mat_val}"

print("Verification passed for n=0..19")

# Show first values
print("\nPaths f(n) for n=0..20:")
for n in range(21):
    print(f"  f({n:2d}) = {solve_dp(n)}")

# Large n example
large_n = 10**18
print(f"\nf({large_n}) mod 10^9+7 = {solve_matrix(large_n, mod)}")

# Forbidden stones example
print(f"\nPaths from 0 to 10 avoiding stones {{3, 7}}: {solve_dp_with_forbidden(10, {3, 7})}")
