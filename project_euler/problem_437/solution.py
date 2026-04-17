"""
Project Euler Problem 437: Fibonacci Primitive Roots

Find the sum of primes < 10^8 with at least one Fibonacci primitive root.

A Fibonacci primitive root g of prime p satisfies g^2 - g - 1 = 0 (mod p)
and g is a primitive root of p.

This requires p ≡ ±1 (mod 5) for sqrt(5) to exist mod p.
"""

import math

def sieve_spf(limit):
    """Sieve of smallest prime factors."""
    spf = list(range(limit))
    for i in range(2, int(limit**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i*i, limit, i):
                if spf[j] == j:
                    spf[j] = i
    return spf

def prime_factors_from_spf(n, spf):
    """Get distinct prime factors using SPF array."""
    factors = []
    while n > 1:
        p = spf[n]
        factors.append(p)
        while n % p == 0:
            n //= p
    return factors

def power_mod(base, exp, mod):
    """Fast modular exponentiation."""
    return pow(base, exp, mod)

def tonelli_shanks(n, p):
    """Compute sqrt(n) mod p using Tonelli-Shanks algorithm."""
    if n == 0:
        return 0
    if power_mod(n, (p - 1) // 2, p) != 1:
        return -1
    if p % 4 == 3:
        return power_mod(n, (p + 1) // 4, p)

    Q, S = p - 1, 0
    while Q % 2 == 0:
        Q //= 2
        S += 1

    z = 2
    while power_mod(z, (p - 1) // 2, p) != p - 1:
        z += 1

    M = S
    c = power_mod(z, Q, p)
    t = power_mod(n, Q, p)
    R = power_mod(n, (Q + 1) // 2, p)

    while True:
        if t == 1:
            return R
        i, tmp = 0, t
        while tmp != 1:
            tmp = tmp * tmp % p
            i += 1
        b = c
        for _ in range(M - i - 1):
            b = b * b % p
        M = i
        c = b * b % p
        t = t * c % p
        R = R * b % p

def is_primitive_root(g, p, factors):
    """Check if g is a primitive root of p."""
    if g == 0:
        return False
    for q in factors:
        if power_mod(g, (p - 1) // q, p) == 1:
            return False
    return True

def solve():
    LIMIT = 100_000_000
    print("Sieving...")
    spf = sieve_spf(LIMIT)

    # Identify primes
    primes = [i for i in range(2, LIMIT) if spf[i] == i]
    print(f"Found {len(primes)} primes")

    ans = 0

    for p in primes:
        if p == 2 or p == 3:
            continue

        if p == 5:
            # g=3: 3^2 - 3 - 1 = 5 ≡ 0 (mod 5), and 3 is a primitive root of 5
            ans += 5
            continue

        r = p % 5
        if r != 1 and r != 4:
            continue

        sqrt5 = tonelli_shanks(5, p)
        if sqrt5 < 0:
            continue

        inv2 = (p + 1) // 2
        g1 = (1 + sqrt5) * inv2 % p
        g2 = (1 + p - sqrt5) * inv2 % p

        factors = prime_factors_from_spf(p - 1, spf)

        if is_primitive_root(g1, p, factors) or is_primitive_root(g2, p, factors):
            ans += p

    print(f"Answer: {ans}")

if __name__ == "__main__":
    solve()
