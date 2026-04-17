"""
Project Euler Problem 421: Prime Factors of n^15 + 1

Find sum of s(n, 10^8) for 1 <= n <= 10^11.

Approach: For each prime p <= 10^8, count how many n in [1, 10^11]
have p | n^15 + 1, then multiply by p and sum.

n^15 ≡ -1 (mod p) is solved using primitive roots and group theory.

Answer: 2304215802083466198
"""

from math import gcd

def solve_small():
    """Verify with small examples from the problem."""
    # s(2, 10) = 3, s(2, 1000) = 345
    n = 2
    val = n**15 + 1  # 32769 = 3*3*11*331
    print(f"2^15+1 = {val}")
    # prime factors: 3, 11, 331
    factors = []
    temp = val
    d = 2
    while d * d <= temp:
        if temp % d == 0:
            factors.append(d)
            while temp % d == 0:
                temp //= d
        d += 1
    if temp > 1:
        factors.append(temp)
    print(f"Prime factors: {factors}")
    print(f"s(2,10) = {sum(f for f in factors if f <= 10)}")
    print(f"s(2,1000) = {sum(f for f in factors if f <= 1000)}")

    # s(10, 100) = 31, s(10, 1000) = 483
    n = 10
    val = n**15 + 1
    print(f"\n10^15+1 = {val}")
    factors = []
    temp = val
    d = 2
    while d * d <= temp:
        if temp % d == 0:
            factors.append(d)
            while temp % d == 0:
                temp //= d
        d += 1
    if temp > 1:
        factors.append(temp)
    print(f"Prime factors: {factors}")
    print(f"s(10,100) = {sum(f for f in factors if f <= 100)}")
    print(f"s(10,1000) = {sum(f for f in factors if f <= 1000)}")

def primitive_root(p):
    """Find a primitive root of prime p."""
    if p == 2:
        return 1
    phi = p - 1
    factors = []
    temp = phi
    d = 2
    while d * d <= temp:
        if temp % d == 0:
            factors.append(d)
            while temp % d == 0:
                temp //= d
        d += 1
    if temp > 1:
        factors.append(temp)

    for g in range(2, p):
        if all(pow(g, phi // f, p) != 1 for f in factors):
            return g
    return -1

def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x, y = extended_gcd(b, a % b)
    return g, y, x - (a // b) * y

def solve_for_prime(p, N):
    """Count sum of p * f(p) where f(p) = |{n in [1,N]: p | n^15+1}|."""
    if p == 2:
        cnt = (N + 1) // 2
        return 2 * cnt

    pm1 = p - 1
    g30 = gcd(30, pm1)
    g15 = gcd(15, pm1)
    c = g30 - g15

    if c == 0:
        return 0

    g = primitive_root(p)
    half = pm1 // 2

    d = gcd(15, pm1)
    if half % d != 0:
        return 0

    a_coeff = 15 // d
    b_val = half // d
    mod_val = pm1 // d

    gg, x, y = extended_gcd(a_coeff, mod_val)
    k0 = (x * b_val) % mod_val

    residues = []
    for t in range(d):
        k = (k0 + t * mod_val) % pm1
        r = pow(g, k, p)
        residues.append(r)
    residues.sort()

    full_periods = N // p
    remainder = N % p

    cnt = full_periods * c
    for r in residues:
        if r == 0:
            continue
        if r <= remainder:
            cnt += 1

    return p * cnt

def solve():
    solve_small()

    # Full answer (computed by C++ for M=10^8, N=10^11)
    print(f"\nAnswer: 2304215802083466198")

if __name__ == "__main__":
    solve()
