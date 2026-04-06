"""
Problem 906: Fibonacci Modular Properties

Find sum_{k=1}^{10^7} F_k mod 10^9+7.

Key identity: sum_{k=1}^{n} F_k = F_{n+2} - 1  (telescoping)

Methods:
    1. Matrix exponentiation: O(log n) time
    2. Fast doubling formulas: O(log n) time, no matrix overhead
    3. Iterative: O(n) time (verification for small n)
"""

MOD = 10**9 + 7

def mat_mul(A, B, mod):
    """Multiply 2x2 matrices mod p."""
    return [
        [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
         (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
        [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
         (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod]
    ]

def mat_pow(M, n, mod):
    """Compute M^n mod p via binary exponentiation."""
    result = [[1, 0], [0, 1]]
    while n > 0:
        if n & 1:
            result = mat_mul(result, M, mod)
        M = mat_mul(M, M, mod)
        n >>= 1
    return result

def fib_matrix(n, mod):
    """Compute F(n) mod p using matrix exponentiation.
    Q^n = [[F(n+1), F(n)], [F(n), F(n-1)]]
    """
    if n <= 0:
        return 0
    if n <= 2:
        return 1
    Q = [[1, 1], [1, 0]]
    R = mat_pow(Q, n - 1, mod)
    return R[0][0]

def fib_doubling(n, mod):
    """Compute (F(n), F(n+1)) mod p using fast doubling.

    F(2k)   = F(k) * (2*F(k+1) - F(k))
    F(2k+1) = F(k)^2 + F(k+1)^2
    """
    if n == 0:
        return (0, 1)

    a, b = fib_doubling(n >> 1, mod)
    c = a * (2 * b - a) % mod
    d = (a * a + b * b) % mod

    if n & 1:
        return (d, (c + d) % mod)
    else:
        return (c, d)

def fib_fast(n, mod):
    """Compute F(n) mod p via fast doubling."""
    return fib_doubling(n, mod)[0]

def fib_sum_iterative(n, mod):
    """Compute sum F(1..n) mod p by iterating."""
    if n <= 0:
        return 0
    total = 0
    a, b = 1, 1  # F(1), F(2)
    for k in range(1, n + 1):
        total = (total + a) % mod
        a, b = b, (a + b) % mod
    return total

# Solve
N = 10**7

fib_np2_mat = fib_matrix(N + 2, MOD)
ans_mat = (fib_np2_mat - 1) % MOD

fib_np2_dbl = fib_fast(N + 2, MOD)
ans_dbl = (fib_np2_dbl - 1) % MOD

assert ans_mat == ans_dbl, f"Mismatch: matrix={ans_mat}, doubling={ans_dbl}"

# Verify telescoping identity for small n
for test_n in [1, 2, 5, 10, 20, 100]:
    s_iter = fib_sum_iterative(test_n, MOD)
    s_formula = (fib_fast(test_n + 2, MOD) - 1) % MOD
    assert s_iter == s_formula, f"n={test_n}: iter={s_iter}, formula={s_formula}"

# Verify known exact values
assert fib_fast(1, MOD) == 1
assert fib_fast(2, MOD) == 1
assert fib_fast(10, MOD) == 55
assert fib_fast(20, MOD) == 6765

print(ans_mat)
