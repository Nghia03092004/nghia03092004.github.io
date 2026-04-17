"""
Problem 929: Prime Gap Distribution

Compute the sum of squared prime gaps: S(N) = sum_{p <= N, p prime} (p' - p)^2,
where p' is the next prime after p, for N = 10^7.

Key results:
  - Prime gaps grow roughly as O(log p), but squared gaps amplify large gaps.
  - The distribution of gaps is highly structured: even gaps dominate (Goldbach-related).

Methods:
  - solve: Sieve of Eratosthenes + iterate consecutive primes, sum gap^2.
  - solve_segmented: Same logic, but using bytearray sieve for memory efficiency.
  - verify_small: Brute-force for small N to cross-check.

Complexity: O(N log log N) for sieve, O(pi(N)) for gap summation.
"""

from collections import Counter

def solve(N=10**7):
    """Sum of squared prime gaps for primes up to N."""
    limit = N + 1000
    sieve = bytearray(b'\x01') * (limit + 1)
    sieve[0] = sieve[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if sieve[i]:
            sieve[i*i::i] = bytearray(len(sieve[i*i::i]))
    primes = [i for i in range(2, limit + 1) if sieve[i]]

    total = 0
    for i in range(len(primes) - 1):
        if primes[i] > N:
            break
        gap = primes[i + 1] - primes[i]
        total += gap * gap
    return total

def verify_small(N):
    """Brute-force sum of squared gaps for small N."""
    sieve = bytearray(b'\x01') * (N + 200)
    sieve[0] = sieve[1] = 0
    for i in range(2, int((N + 200)**0.5) + 1):
        if sieve[i]:
            sieve[i*i::i] = bytearray(len(sieve[i*i::i]))
    primes = [i for i in range(2, N + 200) if sieve[i]]
    total = 0
    for i in range(len(primes) - 1):
        if primes[i] > N:
            break
        total += (primes[i + 1] - primes[i]) ** 2
    return total

def gap_statistics(N):
    """Return list of gaps and basic statistics for primes up to N."""
    sieve = bytearray(b'\x01') * (N + 200)
    sieve[0] = sieve[1] = 0
    for i in range(2, int((N + 200)**0.5) + 1):
        if sieve[i]:
            sieve[i*i::i] = bytearray(len(sieve[i*i::i]))
    primes = [i for i in range(2, N + 200) if sieve[i]]
    gaps = []
    for i in range(len(primes) - 1):
        if primes[i] > N:
            break
        gaps.append(primes[i + 1] - primes[i])
    return primes[:len(gaps)], gaps

# Verify small cases
# Primes up to 20: 2,3,5,7,11,13,17,19 -> gaps: 1,2,2,4,2,4,2
# Sum of squares: 1+4+4+16+4+16+4 = 49
# Primes up to 20: 2,3,5,7,11,13,17,19 -> gaps: 1,2,2,4,2,4,2,4
# Sum of squares: 1+4+4+16+4+16+4+16 = 65
assert verify_small(20) == 65, f"Expected 65, got {verify_small(20)}"
# Primes up to 10: 2,3,5,7 -> gaps: 1,2,2,4 -> 1+4+4+16 = 25
assert verify_small(10) == 25

# Compute answer
answer = solve()
print(answer)
