#!/usr/bin/env python3
"""
Project Euler Problem 457: A Polynomial Modulo the Square of a Prime

f(n) = n^2 - 3n - 1
R(p) = smallest positive n with f(n) = 0 (mod p^2), or 0 if none exists.
SR(L) = sum of R(p) for primes p <= L.

Find SR(10^7).
"""

import math
from time import time

def sieve_primes(limit):
    """Sieve of Eratosthenes returning list of primes up to limit."""
    is_prime = bytearray(b'\x01') * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, limit + 1) if is_prime[i]]

def tonelli_shanks(n, p):
    """Find sqrt(n) mod p using Tonelli-Shanks algorithm. Returns None if no square root."""
    if n % p == 0:
        return 0
    if pow(n, (p - 1) // 2, p) != 1:
        return None  # n is not a QR mod p

    if p % 4 == 3:
        r = pow(n, (p + 1) // 4, p)
        return r

    # Factor out powers of 2 from p-1
    q = p - 1
    s = 0
    while q % 2 == 0:
        q //= 2
        s += 1

    # Find a non-residue z
    z = 2
    while pow(z, (p - 1) // 2, p) != p - 1:
        z += 1

    m = s
    c = pow(z, q, p)
    t = pow(n, q, p)
    r = pow(n, (q + 1) // 2, p)

    while True:
        if t == 1:
            return r
        # Find least i such that t^(2^i) = 1
        i = 1
        temp = (t * t) % p
        while temp != 1:
            temp = (temp * temp) % p
            i += 1
        b = pow(c, 1 << (m - i - 1), p)
        m = i
        c = (b * b) % p
        t = (t * c) % p
        r = (r * b) % p

def solve(L=10**7):
    """Compute SR(L)."""
    t0 = time()
    primes = sieve_primes(L)
    print(f"Sieved {len(primes)} primes up to {L} in {time()-t0:.2f}s")

    total = 0
    count_contributing = 0

    for p in primes:
        if p == 2:
            # Check all n mod 4: f(n) = n^2 - 3n - 1 mod 4
            # f(0)=3, f(1)=1, f(2)=1, f(3)=3 mod 4 => no solution
            continue
        if p == 13:
            # Double root, f'(8) = 0 mod 13, f(8) = 39 != 0 mod 169 => no lift
            continue

        # Check if 13 is a QR mod p
        sq = tonelli_shanks(13, p)
        if sq is None:
            continue

        p2 = p * p
        inv2 = pow(2, p - 2, p)  # 2^(-1) mod p (Fermat's little theorem)

        # Two roots mod p
        r1 = ((3 + sq) * inv2) % p
        r2 = ((3 - sq) * inv2) % p

        best = p2  # Will find minimum positive root mod p^2

        for r in (r1, r2):
            # f(r) mod p should be 0, compute f(r) exactly for lifting
            fr = (r * r - 3 * r - 1) % p2
            fpr = (2 * r - 3) % p  # f'(r) mod p

            if fpr == 0:
                # Degenerate: check if f(r) = 0 mod p^2 already
                # If not, try all lifts r + k*p for k=0..p-1
                for k in range(p):
                    rr = r + k * p
                    if (rr * rr - 3 * rr - 1) % p2 == 0:
                        if rr == 0:
                            rr = p2
                        best = min(best, rr)
                continue

            # Hensel lift: r_lifted = r - f(r) * inv(f'(r)) mod p^2
            inv_fpr = pow(fpr, p - 2, p)
            # Compute lift carefully mod p^2
            # r_lifted = r - f(r) * inv(f'(r)) mod p^2
            # But we need inv(f'(r)) mod p^2 only to first order
            # Actually: lift = r - f(r)/f'(r) mod p^2
            # f(r)/f'(r) needs: f(r) is divisible by p, so f(r)/p is an integer
            # lift = (r - (fr * inv_fpr_p2)) % p2 where inv_fpr_p2 = inv of (2r-3) mod p^2
            # Simpler: use modular inverse mod p^2
            fpr_mod_p2 = (2 * r - 3) % p2
            # inv of fpr_mod_p2 mod p^2, but fpr might not be invertible mod p^2
            # Since fpr != 0 mod p, gcd(fpr_mod_p2, p^2) = 1, so invertible
            inv_fpr_p2 = pow(fpr_mod_p2, -1, p2)
            lifted = (r - fr * inv_fpr_p2) % p2
            if lifted == 0:
                lifted = p2
            best = min(best, lifted)

        if best < p2:
            total += best
            count_contributing += 1

    elapsed = time() - t0
    print(f"Contributing primes: {count_contributing}")
    print(f"SR({L}) = {total}")
    print(f"Time: {elapsed:.2f}s")
    return total

def generate_visualization(L=10**5):
    """Generate visualization of R(p) distribution for primes up to L."""
    primes = sieve_primes(L)

    rp_values = []
    rp_primes = []

    for p in primes:
        if p == 2 or p == 13:
            continue

        sq = tonelli_shanks(13, p)
        if sq is None:
            continue

        p2 = p * p
        inv2 = pow(2, p - 2, p)
        r1 = ((3 + sq) * inv2) % p
        r2 = ((3 - sq) * inv2) % p

        best = p2
        for r in (r1, r2):
            fr = (r * r - 3 * r - 1) % p2
            fpr_mod_p2 = (2 * r - 3) % p2
            fpr = (2 * r - 3) % p
            if fpr == 0:
                for k in range(p):
                    rr = r + k * p
                    if (rr * rr - 3 * rr - 1) % p2 == 0:
                        if rr == 0:
                            rr = p2
                        best = min(best, rr)
                continue
            inv_fpr_p2 = pow(fpr_mod_p2, -1, p2)
            lifted = (r - fr * inv_fpr_p2) % p2
            if lifted == 0:
                lifted = p2
            best = min(best, lifted)

        if best < p2:
            rp_values.append(best)
            rp_primes.append(p)
