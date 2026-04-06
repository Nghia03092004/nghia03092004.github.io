"""
Problem 357: Prime Generating Integers

Find the sum of all n <= 10^8 such that for every divisor d of n,
d + n/d is prime.

Approach:
- n+1 must be prime (d=1 test), so only check n = p-1 for primes p.
- n must be 1 or even.
- For each candidate, check all divisors d <= sqrt(n).

Answer: 1739023853137
"""

import math

def solve():
    LIMIT = 10**8 + 1

    # Sieve of Eratosthenes
    is_prime = bytearray(b'\x01') * (LIMIT + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(math.isqrt(LIMIT)) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    def check(n):
        if n == 1:
            return True
        if n % 2 != 0:
            return False
        d = 2
        while d * d <= n:
            if n % d == 0:
                val = d + n // d
                if val > LIMIT or not is_prime[val]:
                    return False
            d += 1
        return True

    ans = 0
    for p in range(2, LIMIT + 1):
        if is_prime[p]:
            n = p - 1
            if check(n):
                ans += n

    print(ans)

if __name__ == "__main__":
    solve()
