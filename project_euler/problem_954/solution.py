"""
Problem 954: Prime Counting Function

Compute sum_{k=1}^{10^4} pi(k^2) where pi(x) is the prime counting function.

Algorithm:
  1. Sieve of Eratosthenes up to (10^4)^2 = 10^8
  2. Build prefix sum of prime indicator
  3. Sum pi(k^2) for k = 1, ..., 10^4

Complexity: O(M log log M) time, O(M) space, M = 10^8.
"""

from math import isqrt, log

def sieve_prefix(M: int) -> list[int]:
    """Compute prefix sum of prime indicator up to M.

    Returns array P where P[n] = number of primes <= n.
    """
    is_prime = bytearray(b'\x01') * (M + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, isqrt(M) + 1):
        if is_prime[i]:
            is_prime[i * i::i] = bytearray(len(is_prime[i * i::i]))

    # Build prefix sum in-place
    P = [0] * (M + 1)
    for n in range(1, M + 1):
        P[n] = P[n - 1] + is_prime[n]
    return P

def solve(N: int = 10**4) -> int:
    """Compute sum of pi(k^2) for k = 1 to N."""
    M = N * N
    P = sieve_prefix(M)
    return sum(P[k * k] for k in range(1, N + 1))

# --- Compute answer ---
N = 10**4
M = N * N
P = sieve_prefix(M)
answer = sum(P[k * k] for k in range(1, N + 1))

# Verify known values
assert P[100] == 25, f"pi(100) = {P[100]}"
assert P[1000000] == 78498, f"pi(10^6) = {P[1000000]}"

print(answer)
