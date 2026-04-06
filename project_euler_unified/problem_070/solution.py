"""
Project Euler Problem 70: Totient Permutation

Find n, 1 < n < 10^7, for which phi(n) is a digit permutation of n
and n/phi(n) is minimized.

By Theorem 3, primes are excluded. By the Corollary, semiprimes n = p*q
with p, q near sqrt(10^7) are optimal candidates.
"""


def sieve_primes(limit):
    """Sieve of Eratosthenes returning list of primes up to limit."""
    is_prime = bytearray(b'\x01') * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, limit + 1) if is_prime[i]]


def is_permutation(a, b):
    """Check if a and b have the same multiset of decimal digits."""
    return sorted(str(a)) == sorted(str(b))


def main():
    LIMIT = 10_000_000
    primes = sieve_primes(LIMIT)

    best_ratio = float('inf')
    best_n = 0

    for i in range(len(primes)):
        p = primes[i]
        if p * p >= LIMIT:
            break
        for j in range(i, len(primes)):
            q = primes[j]
            n = p * q
            if n >= LIMIT:
                break

            phi = (p - 1) * (q - 1)
            ratio = n / phi

            if ratio < best_ratio and is_permutation(n, phi):
                best_ratio = ratio
                best_n = n

    print(best_n)


if __name__ == "__main__":
    main()
