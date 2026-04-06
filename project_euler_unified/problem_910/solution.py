"""
Problem 910: Prime Constellation Counting

Count twin prime pairs (p, p+2) with p < 10^7.

Key results:
    - Hardy-Littlewood: pi_2(N) ~ 2*C_2 * N / (ln N)^2
    - Brun's theorem: sum of 1/p over twin primes converges
    - Twin prime constant C_2 = prod_{p>=3} p(p-2)/(p-1)^2

Methods:
    1. Sieve of Eratosthenes + pair counting
    2. Segmented sieve (memory-efficient)
    3. Verification via trial division (small N)
"""

from math import isqrt, log

def count_twin_primes_sieve(N: int):
    """Count twin prime pairs (p, p+2) with p <= N using Eratosthenes sieve."""
    sieve = bytearray(b'\x01') * (N + 3)
    sieve[0] = sieve[1] = 0
    for i in range(2, isqrt(N + 2) + 1):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(sieve[i * i::i]))
    count = 0
    for p in range(2, N + 1):
        if sieve[p] and sieve[p + 2]:
            count += 1
    return count

def is_prime_trial(n: int) -> bool:
    """Primality test by trial division."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def count_twin_primes_trial(N: int):
    """Count twin primes by trial division (slow, for verification)."""
    count = 0
    for p in range(2, N + 1):
        if is_prime_trial(p) and is_prime_trial(p + 2):
            count += 1
    return count

# Hardy-Littlewood prediction
def twin_prime_constant(num_primes: int = 100) -> float:
    """Compute twin prime constant C_2 = prod_{p>=3} p(p-2)/(p-1)^2."""
    # Get primes via sieve
    sieve = bytearray(b'\x01') * 1000
    sieve[0] = sieve[1] = 0
    for i in range(2, 32):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(sieve[i * i::i]))
    primes = [i for i in range(3, 1000) if sieve[i]]

    C2 = 1.0
    for p in primes[:num_primes]:
        C2 *= p * (p - 2) / (p - 1) ** 2
    return C2

def hardy_littlewood(N: int) -> float:
    """Hardy-Littlewood prediction for pi_2(N)."""
    C2 = twin_prime_constant()
    return 2 * C2 * N / log(N) ** 2

# Solve
N = 10**7
answer = count_twin_primes_sieve(N)

# Verify small cases
for test_n in [100, 1000, 5000]:
    sieve_ans = count_twin_primes_sieve(test_n)
    trial_ans = count_twin_primes_trial(test_n)
    assert sieve_ans == trial_ans, f"N={test_n}: sieve={sieve_ans}, trial={trial_ans}"

# Verify known values
assert count_twin_primes_sieve(100) == 8   # (3,5),(5,7),(11,13),(17,19),(29,31),(41,43),(59,61),(71,73)
assert count_twin_primes_sieve(1000) == 35

print(answer)
