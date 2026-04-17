"""
Problem 988: |GL(10, 2)| mod 10^9 + 7

Compute the order of the general linear group GL(10, F_2) modulo 10^9 + 7.
GL(n, q) consists of all invertible n x n matrices over the finite field F_q.
Its order is the product: prod_{k=0}^{n-1} (q^n - q^k).

Key results:
    - |GL(n, q)| = prod_{k=0}^{n-1} (q^n - q^k)
    - |GL(10, 2)| = prod_{k=0}^{9} (1024 - 2^k)
    - The probability of a random binary matrix being invertible
      converges to prod_{k=1}^{inf} (1 - 2^{-k}) ~ 0.2888
    - Answer is the exact value modulo 10^9 + 7

Methods:
    1. gl_order_mod       — compute |GL(n, q)| mod m using modular arithmetic
    2. gl_order_exact     — compute exact |GL(n, q)| for small n
    3. invertibility_prob — probability a random matrix over F_q is invertible
    4. factor_analysis    — examine individual (q^n - q^k) factor contributions
"""

MOD = 10**9 + 7

def gl_order_mod(n, q, mod):
    """Compute |GL(n, q)| mod m = prod_{k=0}^{n-1} (q^n - q^k) mod m."""
    result = 1
    for k in range(n):
        factor = (pow(q, n, mod) - pow(q, k, mod)) % mod
        result = (result * factor) % mod
    return result

def gl_order_exact(n, q):
    """Compute exact |GL(n, q)| without modular reduction."""
    result = 1
    qn = q ** n
    for k in range(n):
        result *= (qn - q ** k)
    return result

def invertibility_prob(n, q):
    """P(random n x n matrix over F_q is invertible) = |GL(n,q)| / q^(n^2)."""
    return gl_order_exact(n, q) / q ** (n * n)

def invertibility_limit(q, terms=200):
    """Infinite product limit: prod_{k=1}^{inf} (1 - q^{-k})."""
    result = 1.0
    for k in range(1, terms + 1):
        result *= (1 - q ** (-k))
    return result

def factor_contributions(n, q):
    """Return list of factors (q^n - q^k) for k = 0, ..., n-1."""
    qn = q ** n
    return [qn - q ** k for k in range(n)]

# Verification with known values
# |GL(1, 2)| = 1 (only the 1x1 matrix [1])
assert gl_order_exact(1, 2) == 1

# |GL(2, 2)| = (4-1)(4-2) = 6
assert gl_order_exact(2, 2) == 6

# |GL(3, 2)| = (8-1)(8-2)(8-4) = 7*6*4 = 168
assert gl_order_exact(3, 2) == 168

# Verify modular computation matches exact for small cases
for n in range(1, 8):
    exact = gl_order_exact(n, 2)
    modular = gl_order_mod(n, 2, MOD)
    assert exact % MOD == modular, f"Mismatch at n={n}"

# Cross-check: |GL(10, 2)| exact mod MOD
exact_10 = gl_order_exact(10, 2)
assert exact_10 % MOD == gl_order_mod(10, 2, MOD)

# Compute answer
answer = gl_order_mod(10, 2, MOD)
print(answer)
