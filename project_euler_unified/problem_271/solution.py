"""
Problem 271: Modular Cubes, Part 1

Find the sum of all n (0 < n < N) such that n^3 = 1 (mod N),
where N = 2*3*5*7*11*13*17*19*23*29*31*37*41*43 = 13082761331670030.
"""

from itertools import product
from functools import reduce

def cube_roots_of_unity(p):
    """Find all x with x^3 = 1 (mod p)."""
    roots = [1]
    if p <= 3 or (p - 1) % 3 != 0:
        return roots
    # Find roots of x^2 + x + 1 = 0 (mod p)
    # x = (-1 +/- sqrt(-3)) / 2 mod p
    # Find sqrt(-3) mod p by brute force (p is small)
    neg3 = (-3) % p
    sq = None
    for i in range(p):
        if (i * i) % p == neg3:
            sq = i
            break
    if sq is None:
        return roots
    inv2 = pow(2, -1, p)
    r1 = ((-1 + sq) * inv2) % p
    r2 = ((-1 - sq) * inv2) % p
    if r1 != 1:
        roots.append(r1)
    if r2 != 1:
        roots.append(r2)
    return roots

def crt(residues, moduli):
    """Chinese Remainder Theorem for pairwise coprime moduli."""
    N = reduce(lambda a, b: a * b, moduli)
    result = 0
    for r, m in zip(residues, moduli):
        Mi = N // m
        yi = pow(Mi, -1, m)
        result += r * Mi * yi
    return result % N

def solve():
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43]
    N = reduce(lambda a, b: a * b, primes)

    # Find cube roots of unity for each prime
    roots = [cube_roots_of_unity(p) for p in primes]

    # Enumerate all combinations via CRT
    total = 0
    count = 0
    for combo in product(*roots):
        n = crt(combo, primes)
        if 1 < n < N:
            total += n
            count += 1

    print(total)

if __name__ == "__main__":
    solve()
