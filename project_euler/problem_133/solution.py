"""
Project Euler Problem 133: Repunit Nonfactors

Find the sum of all primes below 100000 that never divide R(10^n).
A prime p (not 2, 3, 5) can divide some R(10^n) iff ord_p(10) is 5-smooth.
"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return [i for i in range(2, limit) if is_prime[i]]

def multiplicative_order(a, n):
    """Compute ord_n(a) by factoring n-1 and refining."""
    if n <= 1:
        return 1
    pm1 = n - 1
    factors = []
    temp = pm1
    f = 2
    while f * f <= temp:
        if temp % f == 0:
            factors.append(f)
            while temp % f == 0:
                temp //= f
        f += 1
    if temp > 1:
        factors.append(temp)
    d = pm1
    for f in factors:
        while d % f == 0 and pow(a, d // f, n) == 1:
            d //= f
    return d

def is_5_smooth(n):
    while n % 2 == 0:
        n //= 2
    while n % 5 == 0:
        n //= 5
    return n == 1

LIMIT = 100000
primes = sieve(LIMIT)
total = 0

for p in primes:
    if p in (2, 3, 5):
        total += p
        continue
    d = multiplicative_order(10, p)
    if not is_5_smooth(d):
        total += p

print(total)
