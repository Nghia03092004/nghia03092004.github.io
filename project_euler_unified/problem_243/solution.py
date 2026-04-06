"""
Problem 243: Resilience

Find the smallest d such that phi(d)/(d-1) < 15499/94744.

Strategy:
1. The ratio phi(d)/d = prod(1 - 1/p) for p | d is minimized by primorial numbers.
2. Find the first primorial P_k with R(P_k) < target.
3. Search d = m * P_j for j < k and small multipliers m.
"""

from fractions import Fraction
from sympy import totient, factorint, primerange

target = Fraction(15499, 94744)

primes = list(primerange(2, 100))

# Compute primorials
primorials = [1]
phi_primorials = [1]
for p in primes:
    primorials.append(primorials[-1] * p)
    phi_primorials.append(phi_primorials[-1] * (p - 1))

# Find first k where R(P_k) < target
first_k = None
for k in range(1, len(primorials)):
    pk = primorials[k]
    phik = phi_primorials[k]
    if Fraction(phik, pk - 1) < target:
        first_k = k
        break

print(f"First primorial below target: P_{first_k} = {primorials[first_k]}")

best_d = primorials[first_k]

# Try d = m * P_j for j < first_k
for j in range(1, first_k):
    pj = primorials[j]
    phij = phi_primorials[j]
    prime_set = set(primes[:j])

    # Case 1: m is smooth w.r.t. P_j's primes
    # phi(m * pj) = m * phij (same prime factors)
    # Need m * phij / (m * pj - 1) < target
    # m * phij * target.denominator < target.numerator * (m * pj - 1)
    # m * (phij * target.den - target.num * pj) < -target.num
    denom = target.numerator * pj - phij * target.denominator
    if denom > 0:
        m_min = target.numerator // denom + 1

        # Find smallest smooth number >= m_min
        for m in range(max(int(m_min), 2), max(int(m_min), 2) + 2000000):
            t = m
            for p in primes[:j]:
                while t % p == 0:
                    t //= p
            if t == 1:
                d = m * pj
                if d < best_d:
                    best_d = d
                    print(f"  Found d = {m} * {pj} = {d} (smooth, j={j})")
                break

    # Case 2: m introduces one new prime
    for pi in range(j, min(j + 6, len(primes))):
        p = primes[pi]
        # phi(d) = m * phij * (p-1)/p  when m = p * s (s smooth)
        # R(d) = m * phij * (p-1) / (p * (m*pj - 1)) < target
        coeff = target.numerator * p * pj - phij * (p - 1) * target.denominator
        if coeff <= 0:
            continue
        m_min2 = target.numerator * p // coeff + 1

        # m must be p * s where s is P_j-smooth, s >= ceil(m_min2 / p)
        s_min = max(1, (int(m_min2) + p - 1) // p)
        for s in range(s_min, s_min + 2000000):
            t = s
            for q in primes[:j]:
                while t % q == 0:
                    t //= q
            if t == 1:
                m = s * p
                d = m * pj
                if d < best_d:
                    best_d = d
                    print(f"  Found d = {m} * {pj} = {d} (new prime {p}, j={j})")
                break

print(f"\nAnswer: {best_d}")
