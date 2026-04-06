"""
Problem 160: Factorial Trailing Digits
Find the last 5 non-trailing-zero digits of (10^12)!
"""

MOD = 100000  # 10^5
MOD2 = 32     # 2^5
MOD5 = 3125   # 5^5

def power(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = result * base % mod
        base = base * base % mod
        exp >>= 1
    return result

def modinv(a, m):
    """Extended GCD to find modular inverse."""
    g, x, _ = extended_gcd(a, m)
    assert g == 1
    return x % m

def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x1, y1 = extended_gcd(b, a % b)
    return g, y1, x1 - (a // b) * y1

def padic_val(n, p):
    """Compute v_p(n!) = sum of floor(n/p^i)."""
    count = 0
    pk = p
    while pk <= n:
        count += n // pk
        pk *= p
    return count

# Precompute partial products of numbers coprime to 5 in [1..3125], mod 3125
partial5 = [0] * (MOD5 + 1)
partial5[0] = 1
for i in range(1, MOD5 + 1):
    if i % 5 != 0:
        partial5[i] = partial5[i-1] * i % MOD5
    else:
        partial5[i] = partial5[i-1]

def coprime5_product(n):
    """Product of k for k=1..n with gcd(k,5)=1, mod 5^5."""
    if n <= 0:
        return 1
    full = n // MOD5
    rem = n % MOD5
    result = power(partial5[MOD5], full, MOD5)
    result = result * partial5[rem] % MOD5
    return result

def fact_remove5(n):
    """n! / 5^{v5(n!)} mod 5^5."""
    if n <= 1:
        return 1
    return coprime5_product(n) * fact_remove5(n // 5) % MOD5

# Precompute partial products with 2s removed in [1..32], mod 32
partial2 = [0] * (MOD2 + 1)
partial2[0] = 1
for i in range(1, MOD2 + 1):
    x = i
    while x % 2 == 0:
        x //= 2
    partial2[i] = partial2[i-1] * x % MOD2

def coprime2_product(n):
    """Product of (k with 2s removed) for k=1..n, mod 2^5."""
    if n <= 0:
        return 1
    full = n // MOD2
    rem = n % MOD2
    result = power(partial2[MOD2], full, MOD2)
    result = result * partial2[rem] % MOD2
    return result

def fact_remove2(n):
    """n! / 2^{v2(n!)} mod 2^5."""
    if n <= 1:
        return 1
    return coprime2_product(n) * fact_remove2(n // 2) % MOD2

def crt(a1, m1, a2, m2):
    """Chinese Remainder Theorem: find x with x=a1 mod m1, x=a2 mod m2."""
    t = (a2 - a1 % m2 + m2) % m2 * modinv(m1 % m2, m2) % m2
    return a1 + m1 * t

N = 10**12

a = padic_val(N, 2)  # v2(N!)
b = padic_val(N, 5)  # v5(N!)

# Compute R = N!/10^b mod 10^5
# R = 2^(a-b) * (N! / (2^a * 5^b))

# mod 5^5: R mod 3125
# N!/5^b mod 5^5 = fact_remove5(N) (includes all factors of 2)
# R = N!/(2^b * 5^b) mod 5^5 = fact_remove5(N) * inv(2^b) mod 5^5
fr5 = fact_remove5(N)
# Euler's totient of 3125 = 2500
R_mod5 = fr5 * modinv(power(2, b % 2500, MOD5), MOD5) % MOD5

# mod 2^5: R mod 32
# N!/2^a mod 32 = fact_remove2(N) (odd, with all 5 factors still in)
# N!/(2^a * 5^b) mod 32 = fact_remove2(N) * inv(5^b mod 32) mod 32
fr2 = fact_remove2(N)
# Euler's totient of 32 = 16
R_mod2 = fr2 * modinv(power(5, b % 16, MOD2), MOD2) % MOD2

# R = 2^(a-b) * (N!/(2^a * 5^b))
# But wait: R = N!/10^b = (N!/(2^a * 5^b)) * 2^(a-b)
# mod 5^5: multiply R_mod5 (which is N!/(2^b*5^b) mod 5^5...
# Actually fr5 = N!/5^b mod 5^5, and I divided by 2^b, so R_mod5 = N!/(2^b*5^b) = N!/10^b mod 5^5. Correct.

# mod 2^5: R_mod2 = N!/(2^a * 5^b) mod 32. But R = 2^(a-b) * that.
e = a - b  # very large, >= 5
if e >= 5:
    pow2e_mod32 = 0
else:
    pow2e_mod32 = 1 << e
R_mod2_final = pow2e_mod32 * R_mod2 % MOD2

answer = crt(R_mod2_final, MOD2, R_mod5, MOD5)
print(answer)
