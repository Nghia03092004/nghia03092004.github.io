"""
Problem 263: An Engineers' Dream Come True

Definitions:
- Practical number: every integer 1..n is a sum of distinct divisors of n.
- Sexy pair: a pair of CONSECUTIVE primes differing by 6. First: (23, 29).
- Triple-pair: three consecutive sexy pairs where second member = first of next.
  i.e., (p, p+6), (p+6, p+12), (p+12, p+18) with all four consecutive primes.
- Engineers' paradise: n such that:
  (1) n-9, n-3, n+3, n+9 are all prime AND are four consecutive primes
      (no primes between them: n-7, n-5, n-1, n+1, n+5, n+7 all composite)
  (2) n-8, n-4, n, n+4, n+8 are all practical numbers

Key observations:
- n must be congruent to 10 or 20 (mod 30).
- The four values are: 219869980, 312501820, 360613700, 1146521020.

Answer: 2039506520
"""

import math

def sieve_primes(limit):
    """Simple sieve of Eratosthenes returning a boolean array."""
    is_prime = bytearray([1]) * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(math.isqrt(limit)) + 1):
        if is_prime[i]:
            for j in range(i*i, limit + 1, i):
                is_prime[j] = 0
    return is_prime

def is_practical(n):
    """Check if n is a practical number using Stewart's criterion."""
    if n <= 0: return False
    if n == 1: return True
    if n % 2 != 0: return False
    factors = []
    temp = n
    d = 2
    while d * d <= temp:
        if temp % d == 0:
            exp = 0
            while temp % d == 0:
                temp //= d
                exp += 1
            factors.append((d, exp))
        d += 1
    if temp > 1:
        factors.append((temp, 1))
    factors.sort()
    if factors[0][0] != 2: return False
    sigma_product = 1
    for i, (p, a) in enumerate(factors):
        if i == 0:
            sigma_product = (p ** (a + 1) - 1) // (p - 1)
        else:
            if p > 1 + sigma_product: return False
            sigma_product *= (p ** (a + 1) - 1) // (p - 1)
    return True

def solve():
    LIMIT = 1_200_000_000

    print("Sieving primes...", flush=True)
    is_prime = sieve_primes(LIMIT + 10)
    print("Sieve done.", flush=True)

    results = []
    n = 10
    while n <= LIMIT and len(results) < 4:
        r = n % 30
        if r == 10 or r == 20:
            # Check four primes
            if (is_prime[n - 9] and is_prime[n - 3] and
                is_prime[n + 3] and is_prime[n + 9]):
                # Check consecutive: no primes between the pairs
                if not (is_prime[n-7] or is_prime[n-5] or
                        is_prime[n-1] or is_prime[n+1] or
                        is_prime[n+5] or is_prime[n+7]):
                    # Check five practical numbers
                    if (is_practical(n - 8) and is_practical(n - 4) and
                        is_practical(n) and is_practical(n + 4) and
                        is_practical(n + 8)):
                        results.append(n)
                        print(f"Found n = {n} ({len(results)}/4)", flush=True)
        n += 10

    print(sum(results))

if __name__ == "__main__":
    solve()
