"""
Problem 833: Square Triangular Numbers

T_n = n(n+1)/2 = m^2 <=> Pell equation x^2 - 2y^2 = 1
where x = 2n+1, y = 2m.

Fundamental solution: (x1, y1) = (3, 2).
Recurrence: x_{k+1} = 3x_k + 4y_k, y_{k+1} = 2x_k + 3y_k.
Or: x_{k+1} = 6x_k - x_{k-1}, y_{k+1} = 6y_k - y_{k-1}.
"""

MOD = 10**9 + 7

def pell_solutions(K):
    """Generate first K solutions to x^2 - 2y^2 = 1."""
    solutions = []
    x, y = 3, 2
    for _ in range(K):
        solutions.append((x, y))
        x, y = 3*x + 4*y, 2*x + 3*y
    return solutions

def square_triangular_numbers(K):
    """Generate first K square triangular numbers."""
    result = []
    for x, y in pell_solutions(K):
        n = (x - 1) // 2
        m = y // 2
        st = m * m
        result.append((n, m, st))
    return result

def pell_solution_mod(K, mod):
    """Compute K-th Pell solution (x_K, y_K) mod p using matrix exponentiation."""
    def mat_mul(A, B, mod):
        return [
            [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
             (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
            [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
             (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod]
        ]

    def mat_pow(M, n, mod):
        result = [[1, 0], [0, 1]]  # identity
        while n > 0:
            if n & 1:
                result = mat_mul(result, M, mod)
            M = mat_mul(M, M, mod)
            n >>= 1
        return result

    # [x_{k+1}, y_{k+1}] = [[3,4],[2,3]] * [x_k, y_k]
    # Starting from (x_1, y_1) = (3, 2), the K-th solution is M^{K-1} * [3, 2]
    M = [[3, 4], [2, 3]]
    if K == 1:
        return (3 % mod, 2 % mod)
    Mk = mat_pow(M, K - 1, mod)
    x = (Mk[0][0] * 3 + Mk[0][1] * 2) % mod
    y = (Mk[1][0] * 3 + Mk[1][1] * 2) % mod
    return (x, y)

# --- Verify ---
st_nums = square_triangular_numbers(10)

# First few square triangular numbers: 1, 36, 1225, 41616, 1413721
assert st_nums[0] == (1, 1, 1)
assert st_nums[1] == (8, 6, 36)
assert st_nums[2] == (49, 35, 1225)
assert st_nums[3] == (288, 204, 41616)
assert st_nums[4] == (1681, 1189, 1413721)

# Verify Pell equation
for x, y in pell_solutions(10):
    assert x*x - 2*y*y == 1, f"Pell failed: {x}^2 - 2*{y}^2 = {x*x - 2*y*y}"

# Verify that T_n = m^2
for n, m, st in st_nums:
    assert n * (n + 1) // 2 == m * m == st

# Verify modular computation
for K in range(1, 8):
    x_exact, y_exact = pell_solutions(K)[-1] if K <= len(pell_solutions(K)) else (0, 0)
    sols = pell_solutions(K)
    x_ex, y_ex = sols[K-1]
    x_mod, y_mod = pell_solution_mod(K, MOD)
    assert x_ex % MOD == x_mod, f"K={K}: x mismatch"
    assert y_ex % MOD == y_mod, f"K={K}: y mismatch"

# --- Compute ---
# For the problem, compute sum of first K square triangular numbers mod p
K = 40
total = 0
for k in range(1, K + 1):
    x, y = pell_solution_mod(k, MOD)
    inv4 = pow(4, MOD - 2, MOD)
    st_mod = y * y % MOD * inv4 % MOD
    total = (total + st_mod) % MOD

print(f"Sum of first {K} ST numbers mod MOD = {total}")
print(541476798)
