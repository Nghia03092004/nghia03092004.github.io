"""
Problem 940: Power Tower Modular Arithmetic

Compute T(2, 100) mod (10^9 + 7), where T(a, n) = a^a^a^...^a (n copies of a)
evaluated right-to-left (power tower / tetration).

Key technique:
    By Euler's theorem, a^x mod m = a^(x mod phi(m)) mod m (when gcd(a,m)=1).
    We recursively reduce the exponent modulo the iterated totient chain:
        m -> phi(m) -> phi(phi(m)) -> ... -> 1
    The chain from 10^9+7 has finite length, so the tower stabilizes.

Results:
    - T(2, 1) = 2
    - T(2, 2) = 4
    - T(2, 3) = 16
    - T(2, 4) = 65536
    - T(2, 100) mod (10^9+7) computed below

Methods:
    1. euler_totient       -- compute phi(n) via trial division
    2. tower_mod           -- recursive power tower modular evaluation
    3. totient_chain       -- iterated totient chain from m down to 1
    4. tower_brute_small   -- brute-force for small cases (verification)
"""

MOD = 10**9 + 7

def euler_totient(n):
    """Compute phi(n) via trial division."""
    result = n
    temp = n
    p = 2
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def tower_mod(a, n, m):
    """Compute a^^n mod m (a^a^...^a, n copies, right-to-left)."""
    if m == 1:
        return 0
    if n == 1:
        return a % m
    phi_m = euler_totient(m)
    exp = tower_mod(a, n - 1, phi_m)
    return pow(a, exp, m)

def totient_chain(m):
    """Return [m, phi(m), phi(phi(m)), ..., 1]."""
    chain = [m]
    x = m
    while x > 1:
        x = euler_totient(x)
        chain.append(x)
    return chain

def tower_brute(a, n):
    """Exact a^^n for small n (no mod). WARNING: grows super-exponentially."""
    if n == 1:
        return a
    return a ** tower_brute(a, n - 1)

# Verification
assert tower_brute(2, 1) == 2
assert tower_brute(2, 2) == 4
assert tower_brute(2, 3) == 16
assert tower_brute(2, 4) == 65536

assert tower_mod(2, 1, MOD) == 2
assert tower_mod(2, 2, MOD) == 4
assert tower_mod(2, 3, MOD) == 16
assert tower_mod(2, 4, MOD) == 65536

# Final answer
answer = tower_mod(2, 100, MOD)
print(answer)
