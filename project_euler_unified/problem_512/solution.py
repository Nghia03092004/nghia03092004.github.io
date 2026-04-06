"""
Problem 512: Sums of Totients of Powers
Compute sum of phi(n^k) for n = 1..N, using phi(n^k) = n^(k-1) * phi(n).
Efficient totient sieve and sublinear summation.
"""

import numpy as np

def totient_sieve(N: int) -> list:
    """
    Compute phi(n) for all n from 0 to N using a sieve.
    O(N log log N) time, O(N) space.
    """
    phi = list(range(N + 1))  # phi[n] = n initially
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, N + 1, p):
                phi[m] = phi[m] // p * (p - 1)
    return phi

def sum_totient_powers_sieve(N: int, k: int):
    """
    Compute sum of phi(n^k) for n = 1..N.
    Uses phi(n^k) = n^(k-1) * phi(n).
    """
    phi = totient_sieve(N)
    total = 0
    for n in range(1, N + 1):
        total += pow(n, k - 1) * phi[n]
    return total

def sum_totient_sublinear(N: int):
    """
    Compute sum of phi(n) for n = 1..N in O(N^{2/3}) time.
    Uses the identity: sum_{n=1}^{N} phi(n) = N*(N+1)/2 - sum_{d=2}^{N} Phi(floor(N/d))
    """
    if N <= 0:
        return 0

    # For small values, use sieve
    cbrt_N = int(N ** (2/3)) + 1
    small_limit = min(cbrt_N, N)
    phi_small = totient_sieve(small_limit)
    prefix = [0] * (small_limit + 1)
    for i in range(1, small_limit + 1):
        prefix[i] = prefix[i-1] + phi_small[i]

    memo = {}

    def Phi(n):
        if n <= small_limit:
            return prefix[n]
        if n in memo:
            return memo[n]

        result = n * (n + 1) // 2
        d = 2
        while d <= n:
            q = n // d
            d_next = n // q + 1
            result -= (d_next - d) * Phi(q)
            d = d_next

        memo[n] = result
        return result

    return Phi(N)

def brute_force_totient_sum(N: int):
    """Brute-force sum of phi(n) for verification."""
    phi = totient_sieve(N)
    return sum(phi[1:N+1])

# Verify sieve
N_test = 100
phi = totient_sieve(N_test)
print("First 20 totient values:")
for n in range(1, 21):
    print(f"  phi({n}) = {phi[n]}")

# Verify sum
sum_sieve = brute_force_totient_sum(N_test)
sum_sub = sum_totient_sublinear(N_test)
print(f"\nSum of phi(n) for n=1..{N_test}:")
print(f"  Sieve: {sum_sieve}")
print(f"  Sublinear: {sum_sub}")
print(f"  Match: {'YES' if sum_sieve == sum_sub else 'NO'}")

# Sum of phi(n^2) = sum of n * phi(n)
N_main = 10000
total_k1 = sum_totient_powers_sieve(N_main, 1)
total_k2 = sum_totient_powers_sieve(N_main, 2)
total_k3 = sum_totient_powers_sieve(N_main, 3)
print(f"\nFor N = {N_main}:")
print(f"  Sum phi(n)   = {total_k1}")
print(f"  Sum phi(n^2) = {total_k2}")
print(f"  Sum phi(n^3) = {total_k3}")

# Sublinear for larger N
N_large = 10**6
large_sum = sum_totient_sublinear(N_large)
print(f"\nSum phi(n) for n=1..{N_large}: {large_sum}")

# Known: sum phi(n) for n=1..N ~ 3N^2/pi^2
approx = 3 * N_large**2 / (np.pi**2)
print(f"  Approximation 3N^2/pi^2 = {approx:.0f}")
print(f"  Ratio: {large_sum / approx:.6f}")
