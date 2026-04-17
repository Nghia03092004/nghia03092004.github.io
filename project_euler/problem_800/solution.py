"""
Project Euler Problem 800: Hybrid Integers

Count hybrid-integers p^q * q^p <= 800800^800800 where p, q are distinct primes.
Using logarithmic transformation: q*ln(p) + p*ln(q) <= 800800*ln(800800)
"""

import math

def sieve(limit):
    """Sieve of Eratosthenes"""
    is_prime = bytearray([1]) * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, limit + 1) if is_prime[i]]

def solve():
    LIMIT = 800800
    L = LIMIT * math.log(LIMIT)

    # For p=2, max q ~ L/ln(2) ~ 15.7M
    SIEVE_LIMIT = 16_000_000
    primes = sieve(SIEVE_LIMIT)

    count = 0
    n = len(primes)
    right = n - 1

    for i in range(n):
        p = primes[i]
        lnp = math.log(p)

        if right <= i:
            right = i + 1

        while right > i:
            q = primes[right]
            val = q * lnp + p * math.log(q)
            if val <= L:
                break
            right -= 1

        if right <= i:
            break

        count += right - i

    print(count)

if __name__ == "__main__":
    solve()
