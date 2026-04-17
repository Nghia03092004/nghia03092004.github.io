"""
Project Euler Problem 282: The Ackermann Function

Find sum_{n=0}^{6} A(n) mod 14^8 where A(n) = ack(n, n).

Closed forms: A(0)=1, A(1)=3, A(2)=7, A(3)=61, A(4)=2^^7 - 3.
A(5) and A(6) involve towers that stabilize modulo 14^8.

Computation via CRT (mod 2^8 and mod 7^8) with iterated Carmichael lambda.
"""

from sympy.ntheory import factorint
from math import gcd

def carmichael(n):
    if n == 1:
        return 1
    result = 1
    factors = factorint(n)
    for p, e in factors.items():
        if p == 2:
            lam = 1 if e == 1 else (2 if e == 2 else p ** (e - 2))
        else:
            lam = (p - 1) * p ** (e - 1)
        result = result * lam // gcd(result, lam)
    return result

def tower2_mod(height, m):
    if m == 1:
        return 0
    if height == 0:
        return 1 % m
    if height == 1:
        return 2 % m

    v2 = 0
    odd_part = m
    while odd_part % 2 == 0:
        odd_part //= 2
        v2 += 1

    # Mod 2^v2
    if v2 == 0:
        mod_2part, p2 = 0, 1
    else:
        p2 = 1 << v2
        small_towers = {0: 1, 1: 2, 2: 4, 3: 16, 4: 65536}
        exp_val = small_towers.get(height - 1, 10**20)
        mod_2part = 0 if exp_val >= v2 else (1 << exp_val) % p2

    # Mod odd_part
    if odd_part == 1:
        mod_oddpart = 0
    else:
        lam = carmichael(odd_part)
        exp_mod = tower2_mod(height - 1, lam)
        mod_oddpart = pow(2, exp_mod, odd_part)

    # CRT
    if v2 == 0:
        return mod_oddpart % m
    inv_p2 = pow(p2, -1, odd_part)
    diff = (mod_oddpart - mod_2part) % odd_part
    t = diff * inv_p2 % odd_part
    return (mod_2part + p2 * t) % m

M = 14 ** 8
a0, a1, a2, a3 = 1, 3, 7, 61
a4 = (tower2_mod(7, M) - 3) % M
stable = tower2_mod(200, M)
a5 = (stable - 3) % M
a6 = a5

print((a0 + a1 + a2 + a3 + a4 + a5 + a6) % M)
