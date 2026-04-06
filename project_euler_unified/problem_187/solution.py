"""
Problem 187: Semiprimes

Count composite integers n < 10^8 with exactly two prime factors.
"""
import bisect

def solve():
    LIMIT = 10**8
    SIEVE_SIZE = LIMIT // 2  # max q when p=2

    # Sieve of Eratosthenes
    is_prime = bytearray(b'\x01') * SIEVE_SIZE
    is_prime[0] = 0
    is_prime[1] = 0

    for i in range(2, int(SIEVE_SIZE**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    primes = [i for i in range(2, SIEVE_SIZE) if is_prime[i]]

    count = 0
    n = len(primes)

    for i, p in enumerate(primes):
        if p * p >= LIMIT:
            break
        max_q = (LIMIT - 1) // p
        # Find number of primes in [p, max_q]
        j = bisect.bisect_right(primes, max_q) - 1
        if j >= i:
            count += j - i + 1

    print(count)

if __name__ == "__main__":
    solve()
