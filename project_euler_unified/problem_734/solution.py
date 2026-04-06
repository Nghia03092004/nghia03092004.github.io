"""
Problem 734: A Bit of Prime

T(n, k) = count of k-tuples of primes <= n whose bitwise OR is also prime <= n.
Uses subset zeta transform and Mobius inversion on the boolean lattice.
"""

MOD = 10**9 + 7

def sieve(n):
    """Sieve of Eratosthenes."""
    is_p = [False, False] + [True] * (n - 1)
    for i in range(2, int(n**0.5) + 1):
        if is_p[i]:
            for j in range(i*i, n+1, i):
                is_p[j] = False
    return is_p

def solve(n, k):
    is_p = sieve(n)
    primes = [p for p in range(2, n+1) if is_p[p]]

    bits = n.bit_length()
    size = 1 << bits

    # f[m] = number of primes that are submasks of m
    f = [0] * size
    for p in primes:
        if p < size:
            f[p] += 1

    # Subset zeta transform: f[m] = sum_{s subset m} f_original[s]
    for i in range(bits):
        for m in range(size):
            if m & (1 << i):
                f[m] += f[m ^ (1 << i)]

    # For each prime q, compute g(q) via Mobius inversion
    total = 0
    for q in primes:
        # Enumerate submasks of q
        gq = 0
        # Use submask enumeration
        s = q
        while True:
            sign = (-1) ** (bin(q).count('1') - bin(s).count('1'))
            fk = pow(f[s], k, MOD)
            gq = (gq + sign * fk) % MOD
            if s == 0:
                break
            s = (s - 1) & q
        total = (total + gq) % MOD

    return total % MOD

# Verify
t5_2 = solve(5, 2)
print(f"T(5, 2) = {t5_2}")  # Expected: 5
assert t5_2 == 5

t100_3 = solve(100, 3)
print(f"T(100, 3) = {t100_3}")  # Expected: 3355

# T(1000, 10) is feasible
t1000_10 = solve(1000, 10)
print(f"T(1000, 10) mod 10^9+7 = {t1000_10}")  # Expected: 2071632

# Full: T(10^6, 999983) - needs optimization for large n
# print(f"T(10^6, 999983) = {solve(10**6, 999983)}")
