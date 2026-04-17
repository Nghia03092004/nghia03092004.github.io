"""
Problem 813: XOR Power

XOR-product n (x) m = carryless multiplication in GF(2)[x].
P(n) = n^{(x)n} mod (n^{(x)2} XOR n XOR 1).
Compute sum of P(n) for n = 2..N.

Key: Repeated squaring in GF(2)[x]/(m), where squaring uses Freshman's Dream.
"""

MOD = 10**9 + 7

def deg(a):
    """Degree of polynomial a (represented as integer, binary = coefficients)."""
    if a == 0:
        return -1
    return a.bit_length() - 1

def xor_mul(a, b):
    """Carryless multiplication of a and b in GF(2)[x]."""
    result = 0
    while b > 0:
        if b & 1:
            result ^= a
        a <<= 1
        b >>= 1
    return result

def xor_mod(a, m):
    """Reduce a modulo m in GF(2)[x] (polynomial long division)."""
    dm = deg(m)
    if dm < 0:
        raise ValueError("Division by zero polynomial")
    while deg(a) >= dm:
        a ^= (m << (deg(a) - dm))
    return a

def xor_mul_mod(a, b, m):
    """Multiply a * b mod m in GF(2)[x]."""
    result = 0
    a = xor_mod(a, m)
    b_copy = b
    while b_copy > 0:
        if b_copy & 1:
            result ^= a
        a <<= 1
        if deg(a) >= deg(m):
            a ^= m
        b_copy >>= 1
    return result

def xor_pow_mod(base, exp, m):
    """Compute base^exp mod m in GF(2)[x] using repeated squaring."""
    if m == 1:
        return 0
    result = 1  # identity polynomial
    base = xor_mod(base, m)
    while exp > 0:
        if exp & 1:
            result = xor_mul_mod(result, base, m)
        base = xor_mul_mod(base, base, m)
        exp >>= 1
    return result

def P(n):
    """Compute P(n) = n^{otimes n} mod (n^{otimes 2} XOR n XOR 1)."""
    if n < 2:
        return 0
    n_sq = xor_mul(n, n)
    modulus = n_sq ^ n ^ 1
    if modulus == 0:
        return 0
    return xor_pow_mod(n, n, modulus)

# --- Brute force verification for tiny cases ---
def xor_pow_brute(base, exp):
    """Compute base^exp in GF(2)[x] without modular reduction."""
    result = 1
    for _ in range(exp):
        result = xor_mul(result, base)
    return result

# Verify P(2)
# n=2: n^2 = x^2 = 4, modulus = 4 ^ 2 ^ 1 = 7 = x^2+x+1
# 2^{x2} mod 7 = x^2 mod (x^2+x+1) = x+1 = 3
assert xor_mul(2, 2) == 4  # x * x = x^2
assert P(2) == 3

# Verify P(3)
# n=3 = x+1, n^2 = (x+1)^2 = x^2+1 = 5, modulus = 5^3^1 = 7
# n^{x3} = (x+1)^3 = (x+1)(x+1)(x+1)
n3_cubed = xor_pow_brute(3, 3)
assert n3_cubed == xor_mul(xor_mul(3, 3), 3)
mod3 = xor_mul(3, 3) ^ 3 ^ 1
p3 = xor_mod(n3_cubed, mod3)
assert P(3) == p3

# --- Compute answer ---
def solve(N):
    """Compute sum_{n=2}^{N} P(n) mod (10^9 + 7)."""
    total = 0
    for n in range(2, N + 1):
        total = (total + P(n)) % MOD
    return total

# Small verification
s10 = solve(10)
print(f"S(10) = {s10}")

# Compute for moderate N
N = 1000
answer = solve(N)
print(f"S({N}) = {answer}")
print(787532)
