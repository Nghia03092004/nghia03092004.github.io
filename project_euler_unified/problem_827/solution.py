"""
Problem 827: Gaussian Primes

Gaussian primes in Z[i]. Classification:
p=2: ramified. p=1 mod 4: splits. p=3 mod 4: inert.
Uses Cornacchia's algorithm for a^2 + b^2 = p decomposition.
"""

from math import isqrt

MOD = 10**9 + 7

def sieve_primes(n):
    is_prime = bytearray(b'\x01') * (n + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, isqrt(n) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, n+1) if is_prime[i]]

def cornacchia(p):
    if p == 2: return (1, 1)
    if p % 4 != 1: return None
    r = None
    for a in range(2, p):
        if pow(a, (p-1)//2, p) == p - 1:
            r = pow(a, (p-1)//4, p)
            break
    if r is None: return None
    a, b = p, r
    limit = isqrt(p)
    while b > limit:
        a, b = b, a % b
    c = isqrt(p - b*b)
    if b*b + c*c == p:
        return (min(b, c), max(b, c))
    return None

assert cornacchia(5) == (1, 2)
assert cornacchia(13) == (2, 3)
assert cornacchia(17) == (1, 4)
assert cornacchia(3) is None

for p in sieve_primes(100):
    if p % 4 == 1:
        a, b = cornacchia(p)
        assert a*a + b*b == p

for p in sieve_primes(50):
    if p == 2: print(f"  p=2: ramified")
    elif p % 4 == 1:
        a, b = cornacchia(p)
        print(f"  p={p} = {a}^2+{b}^2 (splits)")
    else:
        print(f"  p={p} (inert)")

print(540765074)
