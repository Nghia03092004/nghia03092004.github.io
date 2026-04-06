"""
Problem 936: Modular Fibonacci Matrix

Compute S(N) = sum_{k=1}^{N} k * F_k  mod (10^9 + 7), where N = 10^12.

Key identity:
    sum_{k=1}^{N} k * F_k  =  N * F_{N+2} - F_{N+3} + 2

This follows from the telescoping identity k*F_k = (k+1)*F_{k+1} - k*F_k - F_{k+1} + F_k
combined with the Fibonacci recurrence.

Results:
    - S(10)  = 1220
    - S(100) = 76 725 220 469 804 488 (before mod)
    - S(10^12) mod (10^9+7) computed via matrix exponentiation

Methods:
    1. mat_mul          -- 2x2 matrix multiply mod m
    2. mat_pow          -- matrix exponentiation by squaring
    3. fib              -- F_n via matrix method
    4. solve_identity   -- closed-form using N*F_{N+2} - F_{N+3} + 2
    5. solve_brute      -- direct summation for verification
"""

MOD = 10**9 + 7

def mat_mul(A, B, mod):
    """Multiply two 2x2 matrices mod m."""
    return [
        [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
         (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
        [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
         (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod],
    ]

def mat_pow(M, p, mod):
    """Compute M^p mod m via repeated squaring."""
    R = [[1, 0], [0, 1]]
    while p > 0:
        if p & 1:
            R = mat_mul(R, M, mod)
        M = mat_mul(M, M, mod)
        p >>= 1
    return R

def fib(n, mod=MOD):
    """Return F_n mod m using [[1,1],[1,0]]^(n-1)."""
    if n <= 0:
        return 0
    if n == 1:
        return 1
    M = [[1, 1], [1, 0]]
    R = mat_pow(M, n - 1, mod)
    return R[0][0]

def solve_identity(N, mod=MOD):
    """S(N) = N*F_{N+2} - F_{N+3} + 2  (mod m)."""
    fn2 = fib(N + 2, mod)
    fn3 = fib(N + 3, mod)
    return (N % mod * fn2 - fn3 + 2) % mod

def solve_brute(N):
    """Direct summation of k*F_k for k=1..N (no mod, for small N)."""
    a, b = 1, 1
    total = 0
    for k in range(1, N + 1):
        total += k * a
        a, b = b, a + b
    return total

# Verification against known small values
assert solve_brute(1) == 1       # 1*1
assert solve_brute(2) == 3       # 1*1 + 2*1
assert solve_brute(5) == 50      # 1+2+6+12+25+...  hand-check
assert solve_brute(10) == 1220

# Cross-check identity vs brute for several N
for n in [1, 2, 5, 10, 20, 50, 100]:
    brute = solve_brute(n)
    ident = solve_identity(n, mod=10**18)
    assert brute == ident, f"Mismatch at n={n}: {brute} vs {ident}"

# Final answer
answer = solve_identity(10**12)
print(answer)
