"""
Project Euler Problem 451: Modular Inverses

Find sum of I(n) for 3 <= n <= 2*10^7, where I(n) is the largest m < n-1
such that m^2 = 1 (mod n).

Approach: Sieve-based CRT. For each n, we build up the set of self-inverses
by processing one prime-power factor at a time. We store only the set of
known self-inverses so far (from partial factorization).

Answer: 153651073760956
"""

import time
from math import gcd

def compute_I_single(n):
    """Brute-force I(n) for verification: largest m < n-1 with m^2 = 1 mod n."""
    for m in range(n - 2, 0, -1):
        if (m * m) % n == 1:
            return m
    return 0  # only for n <= 2

def solve_sieve(N):
    """
    Compute I(n) for all 3 <= n <= N using a sieve approach.

    For each n, we maintain a list of all self-inverse residues found so far
    (from partial factorization). We process prime powers via a smallest-prime-
    factor sieve, and for each prime power q dividing n, we combine existing
    solutions with the solutions mod q using CRT.

    Returns array result where result[n] = I(n).
    """
    # Step 1: Smallest prime factor sieve
    spf = list(range(N + 1))  # spf[i] = smallest prime factor of i
    for i in range(2, int(N**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, N + 1, i):
                if spf[j] == j:
                    spf[j] = i

    # Step 2: For each n, compute I(n) by factoring via spf and using CRT
    result = [0] * (N + 1)

    for n in range(3, N + 1):
        # Factor n using spf
        temp = n
        prime_powers = []  # list of (p, p^a)
        while temp > 1:
            p = spf[temp]
            pa = 1
            while temp % p == 0:
                pa *= p
                temp //= p
            prime_powers.append((p, pa))

        # For each prime power, find solutions to x^2 = 1 mod p^a
        # Then combine via CRT
        # Solutions mod p^a for odd p: {1, p^a - 1}
        # Solutions mod 2^a: {1} if a=1, {1,3} if a=2, {1, 2^(a-1)-1, 2^(a-1)+1, 2^a-1} if a>=3

        all_solutions = [1]  # start with just x = 1 mod 1

        current_mod = 1
        for p, pa in prime_powers:
            if p == 2:
                if pa == 1:
                    local_sols = [1]
                elif pa == 2:
                    local_sols = [1, 3]
                else:
                    half = pa // 2
                    local_sols = [1, half - 1, half + 1, pa - 1]
            else:
                local_sols = [1, pa - 1]

            # Combine all_solutions (mod current_mod) with local_sols (mod pa) via CRT
            new_solutions = []
            for s1 in all_solutions:
                for s2 in local_sols:
                    # CRT: find x = s1 mod current_mod, x = s2 mod pa
                    combined = crt_two(s1, current_mod, s2, pa)
                    if combined is not None:
                        new_solutions.append(combined)

            current_mod *= pa
            all_solutions = new_solutions

        # I(n) = largest solution that is < n-1 and > 1
        # Solutions include 1 and n-1 always
        best = 1
        for s in all_solutions:
            if 1 < s < n - 1 and s > best:
                best = s

        # If no non-trivial solution exists, I(n) = 1
        result[n] = best

    return result

def extended_gcd(a, b):
    """Extended Euclidean algorithm. Returns (g, x, y) where a*x + b*y = g."""
    if a == 0:
        return b, 0, 1
    g, x, y = extended_gcd(b % a, a)
    return g, y - (b // a) * x, x

def crt_two(r1, m1, r2, m2):
    """
    Chinese Remainder Theorem for two congruences:
    x = r1 mod m1, x = r2 mod m2.
    Returns x mod (m1*m2), or None if no solution.
    Assumes gcd(m1, m2) = 1 (which holds since m1, m2 are coprime prime powers).
    """
    g, p, q = extended_gcd(m1, m2)
    if (r2 - r1) % g != 0:
        return None
    lcm = m1 * m2 // g
    x = (r1 + m1 * ((r2 - r1) // g) * p) % lcm
    return x

def solve_brute(N):
    """Brute force for small N, for verification."""
    result = [0] * (N + 1)
    for n in range(3, N + 1):
        result[n] = compute_I_single(n)
    return result

def create_visualization(N_viz=1000):
    """Create visualization of I(n) values and save to PNG."""
    result = solve_sieve(N_viz)

    ns = list(range(3, N_viz + 1))
    Is = [result[n] for n in ns]
