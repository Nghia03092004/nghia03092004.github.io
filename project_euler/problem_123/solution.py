"""
Problem 123: Prime Square Remainders

Find least n such that ((p_n-1)^n + (p_n+1)^n) mod p_n^2 > 10^10.
For odd n, remainder = 2*n*p_n.  For even n, remainder = 2.
Search for smallest odd n with 2*n*p_n > 10^10.
"""

def sieve(limit):
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit + 1, i):
                is_prime[j] = False
    return [i for i in range(2, limit + 1) if is_prime[i]]

def solve():
    threshold = 10**10
    primes = sieve(300000)
    for i, p in enumerate(primes):
        n = i + 1
        if n % 2 == 0:
            continue
        if 2 * n * p > threshold:
            print(n)
            return

solve()
