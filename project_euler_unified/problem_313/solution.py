"""
Problem 313: Sliding Game

In a sliding puzzle on an m x n grid, a red counter starts at the top-left,
and the empty space starts at the bottom-right. S(m,n) is the minimum number
of moves to slide the red counter to the bottom-right corner.

The problem asks: How many grids (m,n) satisfy S(m,n) = p^2, where p < 10^6 is prime?

Key formulas for S(m,n) with m >= n:
  S(2,2) = 5
  S(3,2) = 9
  S(3,3) = 13
  S(m,2) = 6m - 9  for m > 3
  S(m,n) = S(m,2) + 2(n-2) + 2*[m==n]  for n >= 2

The number of grids with S(m,n) = p^2 for each prime p:
  p = 2: 0 grids (no grid has S = 4)
  p = 3: 2 grids
  p >= 5: (p^2 - 1) / 12 grids

Verification: for primes < 100, total = 5482 (matches problem statement).
"""

def sieve_primes(limit):
    """Sieve of Eratosthenes returning list of primes up to limit (exclusive)."""
    is_prime = bytearray([1]) * limit
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, limit) if is_prime[i]]

def count_grids(p):
    """Number of grids (m,n) with S(m,n) = p^2."""
    if p == 2:
        return 0
    elif p == 3:
        return 2
    else:
        return (p * p - 1) // 12

def solve():
    LIMIT = 1_000_000  # primes < 10^6
    primes = sieve_primes(LIMIT)

    answer = sum(count_grids(p) for p in primes)
    print(answer)

    # Verification for primes < 100
    small_answer = sum(count_grids(p) for p in primes if p < 100)
    assert small_answer == 5482, f"Check failed: {small_answer} != 5482"
